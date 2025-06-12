#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QThread>
#include <QtQmlIntegration/qqmlintegration.h>
#include <optional>
#include <QFileInfo>
#include "types/deviceinfo.h"
#include "decoder.h"
#include <dtracker/audio/engine.hpp>
#include <dtracker/audio/playback_manager.hpp>
#include <dtracker/audio/sample_manager.hpp>
#include <dtracker/tracker/track_manager.hpp>
#include <dtracker/sample/manager.hpp>

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
    ~Manager();

    bool hasDeviceInfo() const;
    Types::DeviceInfo deviceInfo() const;

    // QML-callable methods to control audio playback
    Q_INVOKABLE void startEngine();              // Start the audio engine
    Q_INVOKABLE void startSin();                 // Play a test tone
    Q_INVOKABLE void stopSin();                  // Stop tone playback
    Q_INVOKABLE void playSample(std::vector<float> data, unsigned int rate); // Play decoded sample
    Q_INVOKABLE void playSampleById(int id);
    Q_INVOKABLE void addSampleToTrack(int sampleId, int trackId);
    Q_INVOKABLE void playTrack();
    Q_INVOKABLE dtracker::audio::SampleManager* sampleManager();
    dtracker::tracker::TrackManager* trackManager();

    Q_INVOKABLE void startDecoding(const QString& filePath);
    Q_INVOKABLE void previewSample(const QString& filePath);

signals:
    // Notifies QML when device info becomes available
    void deviceInfoChanged();
    void trackManagerChanged();

    void startDecodingFile(const QString& filePath);
    void fileDecoded(const QString& filePath, std::vector<float> pcmData, dtracker::sample::types::SampleMetadata metaData);

public slots:
    void previewPCMData(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, dtracker::audio::types::AudioProperties properties);

private slots:
    void onDecodingFinished(std::vector<float> pcmData, unsigned int sampleRate, unsigned int sampleBitDepth, QFileInfo fileInfo);

private:
    dtracker::audio::Engine m_engine; // Core audio engine (wraps RtAudio)
    std::optional<Types::DeviceInfo> m_currentDeviceInfo; // Cached output device info

    std::unique_ptr<dtracker::audio::SampleManager> m_sampleManager;
    std::unique_ptr<dtracker::audio::PlaybackManager> m_playbackManager; // Manages active playback units
    std::unique_ptr<dtracker::tracker::TrackManager> m_trackManager;

    dtracker::sample::Manager m_newSampleManager;
    QThread* m_workerThread;
    Decoder* m_audioDecoder;
};

} // namespace Dtracker::Audio

#endif // MANAGER_H
