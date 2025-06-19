#include "playback_facade.h"
#include <QDebug>

PlaybackFacade::PlaybackFacade(QObject *parent)
    : QObject{parent}
{}

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
        m_playbackManager->playTrack(id);
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
