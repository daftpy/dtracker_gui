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

void TrackManager::handleTrackDataModified(Track *track)
{
    if (!m_backend || !track) {
        return;
    }

    qDebug() << "Track" << track->id() << "data modified, syncing to backend...";

    // 1. Get the TrackConfig from the GUI wrapper.
    const TrackConfig& config = track->config();

    // 2. Get the pattern steps from the Qt Model.
    QList<int> qtSteps = track->patternModel()->steps();

    // 3. Convert to the C++ backend format.
    dtracker::tracker::types::ActivePattern cppPattern;
    cppPattern.steps.assign(qtSteps.begin(), qtSteps.end());
    // TODO: Get this from your model in the future.
    cppPattern.stepIntervalMs = 250.0f;

    std::vector<dtracker::tracker::types::ActivePattern> patterns_to_update;
    patterns_to_update.push_back(cppPattern);

    qDebug() << "Updating backend track manager patterns";
    // 4. Call the backend function to update the data model.
    m_backend->updateTrackPatterns(track->id(), patterns_to_update);
}

// Creates a new backend track and corresponding frontend Track object
Track* TrackManager::createTrack(float volume, float pan)
{
    if (!m_backend) {
        qWarning() << "TrackManager: Cannot create track, backend is null.";
        return nullptr;
    }

    // Ask the engine-side TrackManager to create a track and return its ID
    int id = m_backend->createTrack("New Track");

    // Construct config with returned ID and supplied settings
    TrackConfig config(id);
    config.volume = volume;
    config.pan = pan;

    // Create the Track using the config
    auto* track = new Track(config, this);

    connect(track, &Track::dataModified, this, [this, track]() mutable {
        // ...call our handler and tell it which track to sync.
        this->handleTrackDataModified(track);
    });

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
    return true;
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
