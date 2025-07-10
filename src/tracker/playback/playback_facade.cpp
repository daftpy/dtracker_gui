#include "playback_facade.h"
#include <dtracker/audio/playback/buffer_pool.hpp>
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
            emit bpmChanged();
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
            emit bpmChanged();
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
            emit bpmChanged();
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

float PlaybackFacade::bpm() const
{
    if (m_playbackManager != nullptr)
    {
        return m_playbackManager->bpm();
    }
    return 0;
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

void PlaybackFacade::changeBpm(float value)
{
    if (m_playbackManager != nullptr)
    {
        m_playbackManager->setBpm(m_playbackManager->bpm() + value);
        emit bpmChanged();
    }
}

QVariantList PlaybackFacade::waveformData() const
{
    return m_waveformData;
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

    // Get the current playstate from the manager
    bool current = m_playbackManager->isPlaying();

    // Update the value if necessary
    if (current != m_cachedIsPlaying) {
        m_cachedIsPlaying = current;
        emit isPlayingChanged();
    }

    // If playback has stopped, stop polling the playback state
    if (!current) {
        m_playbackStatePoller->stop();
    }

    // --- Waveform Logic ---
    // Get the master mixer waveform data
    auto* queue = m_playbackManager->getMasterWaveformQueue();
    // No data, return
    if (!queue) return;

    QList<QVariant> newPeaks;

    // The queue now holds the shared_ptr objects directly.
    // Use try_pop() to get the object and remove it in one step.
    while (auto* bufferPtrPtr = queue->front()) // bufferPtrPtr is a raw pointer to a shared_ptr
    {
        // 1. Dereference the raw pointer to get the shared_ptr.
        //    We use std::move to efficiently transfer ownership out of the queue.
        dtracker::audio::playback::BufferPool::PooledBufferPtr& bufferPtr = *bufferPtrPtr;

        // 2. Now that we have ownership, pop the raw pointer from the queue.
        queue->pop();

        // 'bufferPtr' is now a PooledBufferPtr, aliasing a std::shared_ptr<PCMData>.
        // When it goes out of scope at the end of this loop, it will automatically
        // return the raw buffer to the BufferPool.
        float minPeak = 0.0f;
        float maxPeak = 0.0f;
        // Dereference the shared_ptr to get the vector for the loop.
        for (float sample : *bufferPtr) {
            if (sample < minPeak) minPeak = sample;
            if (sample > maxPeak) maxPeak = sample;
        }

        qDebug() << "Received Waveform Peak:" << minPeak << maxPeak;
        newPeaks.append(QVariant::fromValue(QList<qreal>{(qreal)minPeak, (qreal)maxPeak}));
    }
    qDebug() << "Done receiving waveform";

    // If we have visual data in newPeaks
    if (!newPeaks.isEmpty()) {

        // Add it to the waveForm data
        m_waveformData.append(newPeaks);

        // Purge old waveForm data
        while (m_waveformData.size() > 300) {
            m_waveformData.removeFirst();
        }
        emit waveformDataChanged();
    }
}
