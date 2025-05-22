#include "manager.h"
#include <QDebug>

namespace Dtracker::Audio {

    Manager::Manager(QObject *parent)
        : QObject{parent}
    {
        // Attempt to start the AudioEngine
        if (!m_engine.start()) {
            qDebug() << "AudioEngine failed to start!";
        }

        // Attempt to retrieve the current device info (optional)
        auto deviceInfoOpt = m_engine.currentDeviceInfo();
        if (deviceInfoOpt.has_value()) {
            // If a valid device was found, convert and store it in m_currentDeviceInfo
            m_currentDeviceInfo = Types::DeviceInfo(*deviceInfoOpt);
        } else {
            // Log a warning if no valid audio output device is available
            qDebug() << "AudioEngine started with no valid device info";
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

}
