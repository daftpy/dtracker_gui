#ifndef SAMPLE_FACADE_H
#define SAMPLE_FACADE_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include "audio/manager.h"
#include "sample_manager_worker.h"
#include "sample_registry_model.h"
#include <memory>

// Orchestrates sample management workflows between the UI, a worker thread, and the audio engine.
// This class acts as a central coordinator, exposing a clean API to QML.
namespace Dtracker::Tracker::Sample {
class SampleFacade : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(SampleFacade)

    // Injected dependency for decoding and playback capabilities.
    Q_PROPERTY(Dtracker::Audio::Manager* audioManager READ audioManager WRITE setAudioManager NOTIFY audioManagerChanged)

    // Exposes the sample list to QML views.
    Q_PROPERTY(SampleRegistryModel* model READ model CONSTANT)
    Q_PROPERTY(dtracker::sample::Manager* manager READ manager CONSTANT)
public:
    explicit SampleFacade(QObject *parent = nullptr);
    ~SampleFacade();

    Dtracker::Audio::Manager* audioManager() const;
    void setAudioManager(Dtracker::Audio::Manager* manager);

    dtracker::sample::Manager* manager();

    // QML-invokable command to begin asynchronous sample registration.
    Q_INVOKABLE void registerSample(const QString& filePath);

    // QML-invokable command to initiate the sample preview workflow.
    Q_INVOKABLE void previewSample(const QString& filePath);
    Q_INVOKABLE void previewSample(int id);

    // Returns the data model for the QML view.
    SampleRegistryModel* model() const { return m_sampleRegistry; }

signals:
    // Notifies when the audioManager dependency has been set.
    void audioManagerChanged();

    // --- Signals to delegate tasks to the worker thread ---
    void checkCache(const QString& filePath);     // Asks the worker to check if a sample is in the cache.
    void requestPCMData(const QString& filePath); // Asks the worker to retrieve PCM data from the cache.
    void addSample(const QString& filePath);      // Asks the worker to register a new sample instance.
    void retrieveSample(int id);

    void playbackSample(dtracker::audio::playback::SamplePlaybackUnit* unit);
    void playbackSampleDescriptor(dtracker::sample::types::SampleDescriptor descriptor);

private slots:
    // --- Slots to handle results from the worker thread ---
    void handleSampleIsCached(const QString& filePath, bool isCached); // Continues the preview workflow after a cache check.
    void handleSampleAdded(int id, const QString& path);               // Updates the QML model after a sample is registered.
    void handleSampleIsFound(dtracker::sample::types::SampleDescriptor descriptor);

private:
    // External dependencies and worker management
    Dtracker::Audio::Manager* m_audioManager{nullptr};      // The audio engine for playback and decoding.
    SampleManagerWorker* m_managerWorker{nullptr};          // The worker object that runs on the background thread.
    QThread* m_workerThread{nullptr};                       // The background thread for all sample management tasks.

    // Owned resources
    std::shared_ptr<dtracker::sample::Manager> m_sampleManager; // The central, shared sample data manager.
    SampleRegistryModel* m_sampleRegistry{new SampleRegistryModel(this)}; // The data model exposed to the UI.
};
}
Q_DECLARE_METATYPE(dtracker::sample::Manager)
#endif // SAMPLE_FACADE_H
