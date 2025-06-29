#include "track.h"

namespace Dtracker::Tracker {

Track::Track(const TrackConfig& config, QObject* parent)
    : QObject(parent), m_config(config)
{
    connect(m_patternModel, &PatternStepModel::stepSet, this,
        [this](int index, int sampleId) {
            if (sampleId >= 0 && !m_config.m_sampleIds.contains(sampleId)) {
                m_config.m_sampleIds.append(sampleId);
                emit sampleIdsChanged();
            }
        });

    connect(m_patternModel, &PatternStepModel::stepSet, this, &Track::dataModified);
}

int Track::id() const
{
    return m_config.id;
}

QString Track::name() const
{
    return m_config.name;
}

void Track::setName(const QString &name)
{
    m_config.name = name;
}

void Track::setVolume(float v)
{
    m_config.volume = v;
}

float Track::pan() const
{
    return m_config.pan;
}

void Track::setPan(float p)
{
    m_config.pan = p;
}

PatternStepModel *Track::patternModel()
{
    return m_patternModel;
}

QList<int> Track::sampleIds() const
{
    return m_config.m_sampleIds;
}

void Track::setSampleIds(const QList<int> &ids)
{
    m_config.m_sampleIds = ids;
}

const TrackConfig &Track::config() const
{
    return m_config;
}

}
