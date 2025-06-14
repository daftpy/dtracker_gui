#include "playback_facade.h"
#include <QDebug>

PlaybackFacade::PlaybackFacade(QObject *parent)
    : QObject{parent}
{}

void PlaybackFacade::setEngine(dtracker::audio::Engine *engine)
{
    if (engine != nullptr) {
        qDebug() << "Engine set and playbackManager created";
        m_engine = engine;
        m_playbackManager = std::make_unique<dtracker::audio::PlaybackManager>(m_engine);
    }
}

void PlaybackFacade::playbackSample(dtracker::audio::playback::SamplePlaybackUnit* unit)
{
    if (m_engine != nullptr && m_playbackManager != nullptr) {
        m_playbackManager->stopPlayback();
        m_playbackManager->playSample(std::unique_ptr<dtracker::audio::playback::SamplePlaybackUnit>(unit));
    }
}

void PlaybackFacade::handlePlaybackSample(dtracker::audio::playback::SamplePlaybackUnit *unit)
{
    playbackSample(unit);
}
