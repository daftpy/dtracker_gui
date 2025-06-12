#ifndef SAMPLE_FACADE_H
#define SAMPLE_FACADE_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include "audio/manager.h"
#include "sample_manager_worker.h"

// This class orchestrates previewing samples by checking if they're cached,
// having them decoded if needed, and playing them back via the audio manager.
namespace Dtracker::Tracker::Sample {
class SampleFacade : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SampleFacade)

    // Allow to be set by QML
    Q_PROPERTY(Dtracker::Audio::Manager* audioManager READ audioManager WRITE setAudioManager NOTIFY audioManagerChanged)
public:
    explicit SampleFacade(QObject *parent = nullptr);

    Dtracker::Audio::Manager* audioManager() const;
    void setAudioManager(Dtracker::Audio::Manager* manager);

public slots:
    // Entry point to preview a sample
    void previewSample(const QString& filePath);

signals:
    void audioManagerChanged();

    // Emitted to check if a sample is already cached
    void checkCache(const QString& filePath);

    // Emitted to request the actual PCM data from cache
    void requestPCMData(const QString& filePath);

private slots:
    // Handles result of the cache check
    void handleSampleIsCached(const QString& filePath, bool isCached);

private:
    Dtracker::Audio::Manager* m_audioManager{nullptr};       // Provides playback and decoding
    SampleManagerWorker* m_managerWorker{nullptr};           // QObject Worker container for SampleManager
    QThread* m_workerThread{nullptr};
};
}

#endif // SAMPLE_FACADE_H
