#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <optional>
#include "types/deviceinfo.h"
#include <dtracker/audio/engine.hpp>
#include <dtracker/audio/playback_manager.hpp>
#include <dtracker/audio/sample_manager.hpp>
#include <dtracker/tracker/track_manager.hpp>

namespace Dtracker::Audio {

class Manager : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AudioManager)

    // QML properties: device info discovery state
    Q_PROPERTY(bool hasDeviceInfo READ hasDeviceInfo NOTIFY deviceInfoChanged)
    Q_PROPERTY(Audio::Types::DeviceInfo deviceInfo READ deviceInfo NOTIFY deviceInfoChanged)
    Q_PROPERTY(dtracker::tracker::TrackManager* trackManager READ trackManager NOTIFY trackManagerChanged)

public:
    explicit Manager(QObject *parent = nullptr);

    bool hasDeviceInfo() const;
    Types::DeviceInfo deviceInfo() const;

    // QML-callable methods to control audio playback
    Q_INVOKABLE void startEngine();              // Start the audio engine
    Q_INVOKABLE void startSin();                 // Play a test tone
    Q_INVOKABLE void stopSin();                  // Stop tone playback
    Q_INVOKABLE void playSample(std::vector<float> data, unsigned int rate); // Play decoded sample
    Q_INVOKABLE void playSampleById(int id);
    Q_INVOKABLE dtracker::audio::SampleManager* sampleManager();
    dtracker::tracker::TrackManager* trackManager();

signals:
    // Notifies QML when device info becomes available
    void deviceInfoChanged();
    void trackManagerChanged();

private:
    dtracker::audio::Engine m_engine; // Core audio engine (wraps RtAudio)
    std::optional<Types::DeviceInfo> m_currentDeviceInfo; // Cached output device info

    std::unique_ptr<dtracker::audio::SampleManager> m_sampleManager;
    std::unique_ptr<dtracker::audio::PlaybackManager> m_playbackManager; // Manages active playback units
    std::unique_ptr<dtracker::tracker::TrackManager> m_trackManager;
};

} // namespace Dtracker::Audio

#endif // MANAGER_H
