#include "manager.h"
#include <QDebug>

namespace Dtracker::Audio {
    Manager::Manager(QObject *parent)
        : QObject{parent}
    {
        if (!m_engine.start()) {
            qDebug() << "AudioEngine failed to start!";
        }
        m_currentDeviceInfo = Types::DeviceInfo(m_engine.currentDeviceInfo());
        qDebug() << "AudioEngine started";
    }

    bool Manager::hasDeviceInfo() const
    {
        return m_currentDeviceInfo.has_value();
    }

    Types::DeviceInfo Manager::deviceInfo() const
    {
        return m_currentDeviceInfo.value_or(Types::DeviceInfo{});
    }
}
