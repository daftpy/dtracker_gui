#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <optional>
#include "types/audiodeviceinfo.h"
#include <engine/audio_engine.hpp>

class AudioManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool hasDeviceInfo READ hasDeviceInfo NOTIFY deviceInfoChanged)
    Q_PROPERTY(AudioDeviceInfo deviceInfo READ deviceInfo NOTIFY deviceInfoChanged)
public:
    explicit AudioManager(QObject *parent = nullptr);

    bool hasDeviceInfo() const;
    AudioDeviceInfo deviceInfo() const;

signals:
    void deviceInfoChanged();

private:
    dtracker::engine::AudioEngine m_engine;
    std::optional<AudioDeviceInfo> m_currentDeviceInfo;
};

#endif // AUDIOMANAGER_H
