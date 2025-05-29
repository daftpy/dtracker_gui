#ifndef SAMPLEENTRY_H
#define SAMPLEENTRY_H

#include <QObject>
#include <QString>
#include <QtQml/qqmlregistration.h>

namespace Dtracker::Tracker {
class SampleEntry
{
    Q_GADGET

    Q_PROPERTY(int id MEMBER id CONSTANT)
    Q_PROPERTY(QString name MEMBER name CONSTANT)
    QML_VALUE_TYPE(sampleEntry)

public:
    SampleEntry() = default;

    int id;
    QString name;
};
}

Q_DECLARE_METATYPE(Dtracker::Tracker::SampleEntry)

#endif // SAMPLEENTRY_H
