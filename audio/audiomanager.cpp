#include "audiomanager.h"
#include <QDebug>

AudioManager::AudioManager(QObject *parent)
    : QObject{parent}
{
    if (!m_engine.start()) {
        qDebug() << "AudioEngine failed to start!";
    }
    m_currentDeviceInfo = AudioDeviceInfo(m_engine.currentDeviceInfo());
    qDebug() << "AudioEngine started";
}

bool AudioManager::hasDeviceInfo() const
{
    return m_currentDeviceInfo.has_value();
}

AudioDeviceInfo AudioManager::deviceInfo() const
{
    return m_currentDeviceInfo.value_or(AudioDeviceInfo{});
}
