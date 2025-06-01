#include "track_manager.h"
#include "track.h"

namespace Dtracker::Tracker {

TrackManager::TrackManager(QObject *parent)
    : QObject(parent),
    m_backend(nullptr),
    m_model(new TrackListModel(this)) // Owned by this TrackManager
{
}

// Returns the pointer to the track list model
TrackListModel* TrackManager::model() const
{
    return m_model;
}

// Creates a new backend track and corresponding frontend Track object
Track* TrackManager::createTrack(float volume, float pan)
{
    if (!m_backend) {
        qWarning() << "TrackManager: Cannot create track, backend is null.";
        return nullptr;
    }

    // Ask the engine-side TrackManager to create a track and return its ID
    int id = m_backend->createTrack(volume, pan);

    // Construct config with returned ID and supplied settings
    TrackConfig config(id);
    config.volume = volume;
    config.pan = pan;

    // Create the Track using the config
    auto* track = new Track(config, this);

    // Add it to the model
    m_model->addTrack(track);

    return track;
}

bool TrackManager::addSamplesToTrack(int trackId, const QList<int> &sampleIds)
{
    if (!m_backend)
        return false;

    // Convert QList to std::vector
    std::vector<int> ids(sampleIds.begin(), sampleIds.end());
    return m_backend->addSamplesToTrack(trackId, ids);
}

// Getter for backend TrackManager (engine-side)
dtracker::tracker::TrackManager* TrackManager::trackManager() const
{
    return m_backend;
}

// Setter for backend TrackManager, emits change signal if updated
void TrackManager::setTrackManager(dtracker::tracker::TrackManager *manager)
{
    if (m_backend == manager)
        return;

    m_backend = manager;
    emit trackManagerChanged(); // Notify QML or other listeners
}

} // namespace Dtracker::Tracker
