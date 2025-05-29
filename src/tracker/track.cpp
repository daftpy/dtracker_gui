#include "track.h"

Track::Track(QObject *parent)
    : QObject{parent}
{}

QString Track::name() const
{
    return m_name;
}

void Track::setName(const QString &name)
{
    m_name = name;
}
