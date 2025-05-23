#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <optional>
#include "types/deviceinfo.h"
#include "audio/engine.hpp"

namespace Dtracker::Audio {
    class Manager : public QObject
    {
        Q_OBJECT
        QML_NAMED_ELEMENT(AudioManager)

        Q_PROPERTY(bool hasDeviceInfo READ hasDeviceInfo NOTIFY deviceInfoChanged)
        Q_PROPERTY(Audio::Types::DeviceInfo deviceInfo READ deviceInfo NOTIFY deviceInfoChanged)
    public:
        explicit Manager(QObject *parent = nullptr);

        bool hasDeviceInfo() const;
        Types::DeviceInfo deviceInfo() const;

        Q_INVOKABLE void startEngine();

    signals:
        void deviceInfoChanged();

    private:
        dtracker::audio::Engine m_engine;
        std::optional<Types::DeviceInfo> m_currentDeviceInfo;
    };

    #endif // MANAGER_H
}
