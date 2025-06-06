#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include <QAbstractListModel>
#include "track.h"
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

namespace Dtracker::Tracker {

// Exposes a list of Track objects to QML
class TrackListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit TrackListModel(QObject *parent = nullptr);

    enum TrackRoles {
        TrackIdRole = Qt::UserRole + 1,   // Unique track ID
        NameRole,                    // Track name
        VolumeRole,                  // Track volume
        PanRole,                     // Track pan
        SampleIdsRole,               // List of sample IDs assigned to this track
        TrackObjectRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    // Adds a Track to the model
    Q_INVOKABLE void addTrack(Track* track);

    // Returns the Track object at a given row, or nullptr if out of bounds
    Q_INVOKABLE Track* get(int row) const;

    // Clears and deletes all Track objects in the model
    Q_INVOKABLE void clear();

private:
    QList<Track*> m_tracks; // Internal storage for all Track objects
};

} // namespace Dtracker::Tracker

#endif // TRACKLISTMODEL_H
