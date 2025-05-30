#include "manager.h"
#include <QDebug>

namespace Dtracker::Audio {

Manager::Manager(QObject *parent)
    : QObject{parent}
{
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

dtracker::audio::SampleManager *Manager::sampleManager()
{
    return m_sampleManager.get();
}

dtracker::tracker::TrackManager *Manager::trackManager()
{
    return m_trackManager.get();
}

} // namespace Dtracker::Audio
