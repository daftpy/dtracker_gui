#ifndef TRACK_CONFIG_H
#define TRACK_CONFIG_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

class TrackConfig
{
    Q_GADGET

    Q_PROPERTY(int id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    Q_PROPERTY(float volume MEMBER volume)
    Q_PROPERTY(float pan MEMBER pan)
    Q_PROPERTY(QList<int> samples MEMBER m_sampleIds)
    QML_VALUE_TYPE(sampleEntry)

public:
    explicit TrackConfig(int id);
    TrackConfig(int id, QString name);

    int id;
    QString name{"New Track"};
    float volume;
    float pan;
    QList<int> m_sampleIds;
};

#endif // TRACK_CONFIG_H
