#include "manager.h"
#include <QDebug>
#include <QThread>
#include <dtracker/sample/types.hpp>

namespace Dtracker::Audio {

Manager::Manager(QObject *parent)
    : QObject{parent}
{
    m_workerThread = new QThread(this);

    // Create DeviceManager using the engine's internal RtAudio instance
    dtracker::audio::DeviceManager dm = m_engine.createDeviceManager();

    // Create the sampleManager responsible for storing sample audio in memory
    m_sampleManager = std::make_unique<dtracker::audio::SampleManager>();

    // Create the track manager, responsible for creating tracks in memory
    m_trackManager = std::make_unique<dtracker::tracker::TrackManager>(m_sampleManager.get());

    // Create the PlaybackManager, responsible for routing decoded samples to the engine
    m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(&m_engine, m_sampleManager.get());

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

// Starts playback of a 440 Hz test tone
void Manager::startSin()
{
    if (m_playbackManager) {
        m_playbackManager->playTestTone(440);
    }
}

// Stops any ongoing playback
void Manager::stopSin()
{
    if (m_playbackManager) {
        m_playbackManager->stopPlayback();
    }
}

// Called when a decoded audio sample is ready
// Forwards it to the playback manager for streaming via RtAudio
void Manager::playSample(std::vector<float> data, unsigned int rate)
{
    qDebug() << "calling dtracker_engine::audio::PlaybackManager playSample";
    qDebug() << "Playing sample of size:" << data.size();

    m_playbackManager->playSample(std::move(data), rate);
}

void Manager::playSampleById(int id)
{
    m_playbackManager->playSampleById(id);
}

void Manager::addSampleToTrack(int sampleId, int trackId)
{
    if (!m_trackManager)
        return;

    // Wrap the sampleId in a vector since addSamplesToTrack expects a list
    std::vector<int> sampleIds = {sampleId};
    bool success = m_trackManager->addSamplesToTrack(0, sampleIds);

    if (!success) {
        qDebug() << "Failed to add sample" << sampleId << "to track" << 0;
    }
}

void Manager::playTrack()
{
    auto* track = m_trackManager->getTrack(0);
    if (track) {
        qDebug() << "track found";
        m_engine.mixerUnit()->clear();   // Remove any leftover units
        track->reset();                  // Restart the track
        // m_engine.mixerUnit()->addUnit(track); // Add again
    }
}

dtracker::audio::SampleManager *Manager::sampleManager()
{
    return m_sampleManager.get();
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

void Manager::onDecodingFinished(std::vector<float> pcmData, unsigned int sampleRate, unsigned int sampleBitDepth, QFileInfo fileInfo)
{
    qDebug() << "Finished decoding file " << fileInfo.absoluteFilePath();
    dtracker::sample::types::SampleMetadata meta;
    meta.sourceSampleRate = sampleRate;
    meta.bitDepth = sampleBitDepth;
    auto id = m_newSampleManager.addSample(fileInfo.absoluteFilePath().toStdString(), pcmData, meta);
    qDebug() << "Sample addedd, returned id:" << id;
}

} // namespace Dtracker::Audio
