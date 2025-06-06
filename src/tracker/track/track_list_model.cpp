#include "track_list_model.h"

namespace Dtracker::Tracker {

TrackListModel::TrackListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int TrackListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_tracks.size();
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    // Validate index bounds
    if (!index.isValid() || index.row() < 0 || index.row() >= m_tracks.size())
        return QVariant();

    const Track* track = m_tracks.at(index.row());

    switch (role) {
    case TrackIdRole:
        return track->id();
    case NameRole:
        return track->name();
    case VolumeRole:
        return track->volume();
    case PanRole:
        return track->pan();
    case SampleIdsRole:
        return QVariant::fromValue(track->sampleIds());
    case TrackObjectRole:
        return QVariant::fromValue(track);
    default:
        return QVariant(); // Unknown role
    }
}

QHash<int, QByteArray> TrackListModel::roleNames() const
{
    return {
        {TrackIdRole, "trackId"},
        {NameRole, "name"},
        {VolumeRole, "volume"},
        {PanRole, "pan"},
        {SampleIdsRole, "sampleIds"},
        {TrackObjectRole, "track"}
    };
}

void TrackListModel::addTrack(Track *track)
{
    beginInsertRows(QModelIndex(), m_tracks.size(), m_tracks.size());

    m_tracks.append(track);

    endInsertRows();
}

Track* TrackListModel::get(int row) const
{
    if (row < 0 || row >= m_tracks.size())
        return nullptr;

    // Return the Track pointer at the requested row
    return m_tracks.at(row);
}

void TrackListModel::clear()
{
    beginResetModel();

    // Delete all Track objects and clear the list
    qDeleteAll(m_tracks); // Deletes the QObject pointers safely
    m_tracks.clear();

    endResetModel();
}

} // namespace Dtracker::Tracker
