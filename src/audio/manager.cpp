#include "manager.h"
#include <QDebug>
#include <QThread>
#include <dtracker/sample/types.hpp>
#include <dtracker/audio/playback/sample_playback_unit.hpp>

namespace Dtracker::Audio {

Manager::Manager(QObject *parent)
    : QObject{parent}
{
    m_workerThread = new QThread(this);

    // Create DeviceManager using the engine's internal RtAudio instance
    dtracker::audio::DeviceManager dm = m_engine.createDeviceManager();

    // Create the track manager, responsible for creating tracks in memory
    m_trackManager = std::make_unique<dtracker::tracker::TrackManager>();

    // Create the PlaybackManager, responsible for routing decoded samples to the engine
    m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(&m_engine);

    // Attempt to find and set a valid output device
    if (dm.currentDeviceInfo().has_value()) {
        auto deviceInfo = dm.currentDeviceInfo().value();
        m_currentDeviceInfo = Types::DeviceInfo(deviceInfo);

        m_engine.setOutputDevice(deviceInfo.ID);

        // Start the audio engine after setting the device
        if (!m_engine.start()) {
            qDebug() << "AudioEngine failed to start!";
        }
    } else {
        qDebug() << "AudioEngine failed to start due to no usable audio device";
    }

    m_audioDecoder = new Dtracker::Audio::Decoder();

    m_audioDecoder->moveToThread(m_workerThread);

    // --- Connect signals and slots ---
    connect(m_workerThread, &QThread::finished, m_audioDecoder, &QObject::deleteLater);
    connect(this, &Manager::startDecodingFile, m_audioDecoder, &Dtracker::Audio::Decoder::startDecodingFile);
    connect(m_audioDecoder, &Dtracker::Audio::Decoder::sampleReady, this, &Manager::onDecodingFinished);

    m_workerThread->start();
}

Manager::~Manager()
{
    m_workerThread->quit();

    // A short timeout is added as a safeguard in case the thread is stuck
    if (!m_workerThread->wait(3000)) // Wait for up to 3 seconds
    {
        // If the thread did not shut down gracefully, forcefully terminate it
        qDebug() << "Warning: Worker thread did not shut down in time. Forcing termination.";
        m_workerThread->terminate();
        m_workerThread->wait(); // Wait for the termination to complete.
    }
}

// Returns true if a usable output device has been found
bool Manager::hasDeviceInfo() const
{
    return m_currentDeviceInfo.has_value();
}

// Returns the stored output device info, or a fallback default if none
Types::DeviceInfo Manager::deviceInfo() const
{
    return m_currentDeviceInfo.value_or(Types::DeviceInfo{});
}

// Starts the engine if it's not already running
void Manager::startEngine()
{
    if (m_engine.isStreamRunning())
        return;

    if (!m_engine.start()) {
        qDebug() << "AudioEngine failed to start!";
    }
}

dtracker::tracker::TrackManager *Manager::trackManager()
{
    return m_trackManager.get();
}

void Manager::startDecoding(const QString& filePath)
{
    qDebug() << "Starting decoding process";
    emit startDecodingFile(filePath);
}

void Manager::onDecodingFinished(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, unsigned int sampleRate, unsigned int sampleBitDepth, QFileInfo fileInfo)
{
    qDebug() << "Finished decoding file " << fileInfo.absoluteFilePath();

    // 1. Create the SampleMetadata
    dtracker::sample::types::SampleMetadata meta;
    meta.sourceSampleRate = sampleRate;
    meta.bitDepth = sampleBitDepth;

    // 2. Create the descriptor
    dtracker::sample::types::SampleDescriptor descriptor(-1, pcmData, meta);

    // 3. Create the playback unit using the helper
    std::unique_ptr<dtracker::audio::playback::SamplePlaybackUnit> unit = dtracker::audio::playback::makePlaybackUnit(descriptor);

    // 4. Clear the mixer (stop sound)
    m_engine.mixerUnit()->clear();

    // 5. Move the unit to the mixer (preview)
    m_engine.mixerUnit()->addUnit(std::move(unit));

    // Emit the file has been decoded with the pcm and meta data
    emit fileDecoded(fileInfo.absoluteFilePath(), std::move(pcmData), meta);
}

void Manager::previewPCMData(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, dtracker::audio::types::AudioProperties properties)
{
    // 1. Create the meta data
    dtracker::sample::types::SampleMetadata meta;
    meta.sourceSampleRate = properties.sampleRate;
    meta.bitDepth = properties.bitDepth;

    // 2. Create the descriptor
    dtracker::sample::types::SampleDescriptor descriptor(-1, std::move(pcmData), meta);

    // 3. Create the playback unit using the helper
    auto unit = dtracker::audio::playback::makePlaybackUnit(descriptor);

    // 4. Clear the mixer (stop sound)
    m_engine.mixerUnit()->clear();

    // 5. Move the unit to the mixer (preview)
    m_engine.mixerUnit()->addUnit(std::move(unit));
}

} // namespace Dtracker::Audio
