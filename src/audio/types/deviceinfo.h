#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QMetaType>
#include <QString>
#include <QVector>
#include <RtAudio.h>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Dtracker::Audio::Types {
    class DeviceInfo
    {
        Q_GADGET
        Q_PROPERTY(QString name MEMBER name CONSTANT)
        Q_PROPERTY(unsigned int outputChannels MEMBER outputChannels CONSTANT)
        Q_PROPERTY(unsigned int inputChannels MEMBER inputChannels CONSTANT)
        Q_PROPERTY(unsigned int duplexChannels MEMBER duplexChannels CONSTANT)
        Q_PROPERTY(bool isDefaultOutput MEMBER isDefaultOutput CONSTANT)
        Q_PROPERTY(bool isDefaultInput MEMBER isDefaultInput CONSTANT)
        Q_PROPERTY(QList<unsigned int> sampleRates MEMBER sampleRates CONSTANT)
        Q_PROPERTY(unsigned int preferredSampleRate MEMBER preferredSampleRate CONSTANT)
        QML_VALUE_TYPE(audioDeviceInfo)
    public:
        DeviceInfo();
        explicit DeviceInfo(const RtAudio::DeviceInfo& info);

        QString name;
        unsigned int outputChannels;
        unsigned int inputChannels;
        unsigned int duplexChannels;
        bool isDefaultOutput;
        bool isDefaultInput;
        QList<unsigned int> sampleRates;
        unsigned int preferredSampleRate;
    };
}

#endif // DEVICEINFO_H
