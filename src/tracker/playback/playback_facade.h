#ifndef PLAYBACK_FACADE_H
#define PLAYBACK_FACADE_H

#include <QObject>
#include <QQmlEngine>
#include <memory>
#include <dtracker/audio/engine.hpp>
#include <dtracker/audio/playback_manager.hpp>
#include <dtracker/audio/playback/sample_playback_unit.hpp>
#include <dtracker/sample/manager.hpp>
#include <dtracker/tracker/track_manager.hpp>

class PlaybackFacade : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(dtracker::audio::Engine* engine READ engine WRITE setEngine NOTIFY engineChanged)
    Q_PROPERTY(dtracker::sample::Manager* sampleManager READ sampleManager WRITE setSampleManager NOTIFY sampleManagerChanged)
    Q_PROPERTY(dtracker::tracker::TrackManager* trackManager READ trackManager WRITE setTrackManager NOTIFY trackManagerChanged)
public:
    explicit PlaybackFacade(QObject *parent = nullptr);

    dtracker::audio::Engine* engine() const { return m_engine; };
    void setEngine(dtracker::audio::Engine* engine);

    dtracker::sample::Manager* sampleManager() const { return m_sampleManager; };
    void setSampleManager(dtracker::sample::Manager* manager);

    dtracker::tracker::TrackManager* trackManager() const { return m_trackManager; };
    void setTrackManager(dtracker::tracker::TrackManager* manager);

    Q_INVOKABLE void playbackSample(dtracker::audio::playback::SamplePlaybackUnit* unit);

    Q_INVOKABLE void playTrack(int id);

    // TODO: stop playback

signals:
    void engineChanged();
    void sampleManagerChanged();
    void trackManagerChanged();

public slots:
    void handlePlaybackSample(dtracker::audio::playback::SamplePlaybackUnit* unit);

    void handlePlaybackSampleDescriptor(dtracker::sample::types::SampleDescriptor descriptor);

private:
    dtracker::audio::Engine* m_engine{nullptr};
    dtracker::sample::Manager* m_sampleManager{nullptr};
    dtracker::tracker::TrackManager* m_trackManager{nullptr};

    std::unique_ptr<dtracker::audio::PlaybackManager> m_playbackManager;
};

#endif // PLAYBACK_FACADE_H
