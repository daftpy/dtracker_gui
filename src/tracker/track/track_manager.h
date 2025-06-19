#ifndef TRACKMANAGER_H
#define TRACKMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

#include "track_list_model.h"
#include <dtracker/tracker/track_manager.hpp>
#include <dtracker/tracker/types.hpp>

namespace Dtracker::Tracker {

// Acts as the Qt/QML-facing controller for managing tracks
class TrackManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Exposes the list model of tracks to QML (read-only)
    Q_PROPERTY(TrackListModel* model READ model CONSTANT)

    // Allows the engine-side TrackManager to be injected/set from QML or C++
    Q_PROPERTY(dtracker::tracker::TrackManager* trackManager READ trackManager WRITE setTrackManager NOTIFY trackManagerChanged)

public:
    explicit TrackManager(QObject* parent = nullptr);

    // Creates a new Track instance (Qt side) and corresponding backend track
    Q_INVOKABLE Track* createTrack(float volume = 1.0f, float pan = 0.0f);

    // Adds sample(s) to an existing track by ID from QML
    Q_INVOKABLE bool addSamplesToTrack(int trackId, const QList<int>& sampleIds);

    // Returns pointer to the current engine-side TrackManager
    dtracker::tracker::TrackManager* trackManager() const;

    // Sets the engine-side TrackManager (from audioManager)
    void setTrackManager(dtracker::tracker::TrackManager* manager);

    // Returns the list model used by QML to display/edit tracks
    TrackListModel* model() const;

signals:
    // Emitted when the engine-side TrackManager is changed
    void trackManagerChanged();

public slots:
    void handleTrackDataModified(Dtracker::Tracker::Track* track);

private:
    // Holds the QML model of tracks
    TrackListModel* m_model;

    // Raw pointer to the engine-side TrackManager
    dtracker::tracker::TrackManager* m_backend{nullptr};
};

} // namespace Dtracker::Tracker

// Registers the TrackManager pointer type for QVariant/QML use
Q_DECLARE_METATYPE(dtracker::tracker::TrackManager)

#endif // TRACKMANAGER_H
