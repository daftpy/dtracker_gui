#ifndef PLAYBACK_FACADE_H
#define PLAYBACK_FACADE_H

#include <QObject>
#include <QQmlEngine>
#include <memory>
#include <dtracker/audio/engine.hpp>
#include <dtracker/audio/playback_manager.hpp>
#include <dtracker/audio/playback/sample_playback_unit.hpp>

class PlaybackFacade : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(dtracker::audio::Engine* engine READ engine WRITE setEngine NOTIFY engineChanged)
public:
    explicit PlaybackFacade(QObject *parent = nullptr);

    dtracker::audio::Engine* engine() const { return m_engine; };
    void setEngine(dtracker::audio::Engine* engine);

    Q_INVOKABLE void playbackSample(dtracker::audio::playback::SamplePlaybackUnit* unit);

    // TODO: stop playback

signals:
    void engineChanged();

public slots:
    void handlePlaybackSample(dtracker::audio::playback::SamplePlaybackUnit* unit);

private:
    dtracker::audio::Engine* m_engine;
    std::unique_ptr<dtracker::audio::PlaybackManager> m_playbackManager;
};

#endif // PLAYBACK_FACADE_H
