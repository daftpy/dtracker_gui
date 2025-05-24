#include "manager.h"
#include <QDebug>

namespace Dtracker::Audio {

    Manager::Manager(QObject *parent)
        : QObject{parent}
    {
        // Create the discovery manager
        dtracker::audio::DeviceManager dm = m_engine.createDeviceManager();
        //dtracker::audio::PlaybackManager pm = dtracker::audio::PlaybackManager(&m_engine);
        m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(&m_engine);

        if (dm.currentDeviceInfo().has_value()) {
            auto deviceInfo = dm.currentDeviceInfo().value();
            m_currentDeviceInfo = Types::DeviceInfo(deviceInfo);

            m_engine.setOutputDevice(deviceInfo.ID);

            // Attempt to start the AudioEngine
            if (!m_engine.start()) {
                qDebug() << "AudioEngine failed to start!";
            }
        } else {
            qDebug() << "AudioEngine failed to start due to no usable audio device";
        }
    }

    // Expose whether we currently have device info
    bool Manager::hasDeviceInfo() const
    {
        return m_currentDeviceInfo.has_value();
    }

    // Return the stored device info, or a default-constructed fallback if none available
    Types::DeviceInfo Manager::deviceInfo() const
    {
        return m_currentDeviceInfo.value_or(Types::DeviceInfo{});
    }

    void Manager::startEngine()
    {
        if (m_engine.isStreamRunning())
            return;

        if (!m_engine.start()) {
            qDebug() << "AudioEngine failed to start!";
        }
    }

    void Manager::startSin()
    {
        if (m_playbackManager) {
            m_playbackManager.get()->playTestTone(440);
        }
    }

    void Manager::stopSin()
    {
        if (m_playbackManager) {
            m_playbackManager.get()->stopPlayback();
        }
    }

    void Manager::playSample(std::vector<float> data, unsigned int rate)
    {
        qDebug() << "calling dtracker_engine::audio::PlaybackManager playSample";
        qDebug() << "Playing sample of size:" << data.size();

        m_playbackManager->playSample(std::move(data), rate);
    }

}
