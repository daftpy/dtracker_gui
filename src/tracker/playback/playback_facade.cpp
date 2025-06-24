#include "playback_facade.h"
#include <QDebug>

PlaybackFacade::PlaybackFacade(QObject *parent)
    : QObject{parent}
{
    connect(m_playbackStatePoller, &QTimer::timeout, this, &PlaybackFacade::updateIsPlayingState);
}

void PlaybackFacade::setEngine(dtracker::audio::Engine *engine)
{
    if (engine != nullptr) {
        qDebug() << "Engine set";
        m_engine = engine;
        emit engineChanged();

        if (m_engine != nullptr && m_sampleManager != nullptr && m_trackManager != nullptr) {
            qDebug() << "PlaybackManager initialized set";
            m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(m_engine, m_sampleManager, m_trackManager);
        }
    }
}

void PlaybackFacade::setSampleManager(dtracker::sample::Manager *manager)
{
    if (manager != nullptr) {

        qDebug() << "Sample Manager set";
        m_sampleManager = manager;
        emit sampleManagerChanged();

        if (m_engine != nullptr && m_sampleManager != nullptr && m_trackManager != nullptr) {
            qDebug() << "PlaybackManager initialized set";
            m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(m_engine, m_sampleManager, m_trackManager);
        }
    }
}

void PlaybackFacade::setTrackManager(dtracker::tracker::TrackManager *manager)
{
    if (manager != nullptr)
    {
        qDebug() << "Track Manager set";
        m_trackManager = manager;
        emit trackManagerChanged();

        if (m_engine != nullptr && m_sampleManager != nullptr && m_trackManager != nullptr) {
            qDebug() << "PlaybackManager initialized set";
            m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(m_engine, m_sampleManager, m_trackManager);
        }
    }
}

bool PlaybackFacade::loopPlayback() const
{
    if (m_playbackManager != nullptr) {
        return m_playbackManager->loopPlayback();
    }
    return false;
}

void PlaybackFacade::setLoopPlayback(bool shouldLoop)
{
    if (m_playbackManager != nullptr) {
        m_playbackManager->setLoopPlayback(shouldLoop);
        emit loopPlaybackChanged();
    }
}

bool PlaybackFacade::isPlaying() const
{
    return m_cachedIsPlaying;
}

void PlaybackFacade::playbackSample(dtracker::audio::playback::SamplePlaybackUnit* unit)
{
    if (m_engine != nullptr && m_playbackManager != nullptr) {
        m_playbackManager->stopPlayback();
        m_playbackManager->playSample(std::unique_ptr<dtracker::audio::playback::SamplePlaybackUnit>(unit));
    }
}

void PlaybackFacade::playTrack(int id)
{
    if (m_playbackManager != nullptr)
    {
        qDebug() << "Playing track" << id;
        // m_playbackManager->playTrack(id);
        m_playbackStatePoller->start(100);
        m_playbackManager->playAllTracks();
    }
}

void PlaybackFacade::stopPlayback()
{
    if (m_playbackManager != nullptr)
    {
        m_playbackManager->stopPlayback();
    }
}

void PlaybackFacade::handlePlaybackSample(dtracker::audio::playback::SamplePlaybackUnit *unit)
{
    playbackSample(unit);
}

void PlaybackFacade::handlePlaybackSampleDescriptor(dtracker::sample::types::SampleDescriptor descriptor)
{
    qDebug() << "Playing sample descriptor using recycled units";
    m_playbackManager->playSample(descriptor);
}

void PlaybackFacade::handleIsPlayingChanged()
{
    if (m_playbackManager) {
        if (!m_playbackManager->isPlaying())
            m_playbackStatePoller->stop();
    }
}

void PlaybackFacade::updateIsPlayingState()
{
    if (!m_playbackManager) return;

    bool current = m_playbackManager->isPlaying();
    if (current != m_cachedIsPlaying) {
        m_cachedIsPlaying = current;
        qDebug() << "Is playing changed! :" << m_cachedIsPlaying;
        emit isPlayingChanged();
    }
}
