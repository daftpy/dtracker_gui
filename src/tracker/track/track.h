#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QString>
#include <QList>
#include "pattern/pattern_step_model.h"
#include "types/track_config.h"
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Dtracker::Tracker {

// Track class represents a single user-facing track in the DAW.
// It wraps a TrackConfig (a plain data struct) and exposes it to QML.
class Track : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Read-only track ID
    Q_PROPERTY(int trackId READ id CONSTANT)

    // Editable track name
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    // Volume control (0.0 to 1.0)
    Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)

    // Stereo pan control (-1.0 = left, 0.0 = center, 1.0 = right)
    Q_PROPERTY(float pan READ pan WRITE setPan NOTIFY panChanged)

    // List of sample IDs this track should trigger
    Q_PROPERTY(QList<int> sampleIds READ sampleIds WRITE setSampleIds NOTIFY sampleIdsChanged)

    Q_PROPERTY(PatternStepModel* patternModel READ patternModel CONSTANT)

public:
    explicit Track(const TrackConfig& config, QObject* parent = nullptr);

    // Returns the internal track ID
    int id() const;

    // Track name accessors
    QString name() const;
    void setName(const QString& name);

    // Volume accessors
    float volume() const { return m_config.volume; }
    void setVolume(float v);

    // Pan accessors
    float pan() const;
    void setPan(float p);

    PatternStepModel* patternModel();

    // Sample ID list accessors
    QList<int> sampleIds() const;
    void setSampleIds(const QList<int>& ids);

    // Returns the entire underlying config
    const TrackConfig& config() const;

signals:
    void nameChanged();
    void volumeChanged();
    void panChanged();
    void sampleIdsChanged();

private:
    TrackConfig m_config;
    PatternStepModel* m_patternModel{new PatternStepModel(this)};
};

} // namespace Dtracker::Tracker

#endif // TRACK_H
