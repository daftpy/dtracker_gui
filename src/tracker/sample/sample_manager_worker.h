#ifndef SAMPLE_MANAGER_WORKER_H
#define SAMPLE_MANAGER_WORKER_H

#include <QObject>
#include <dtracker/sample/manager.hpp>
#include <dtracker/sample/types.hpp>
#include <dtracker/audio/types.hpp>
#include <QMetaType>

namespace Dtracker::Tracker::Sample {

// This worker runs in a separate thread to offload potentially expensive
// cache operations (e.g., inserting or fetching PCM data).
class SampleManagerWorker : public QObject
{
    Q_OBJECT
public:
    explicit SampleManagerWorker(QObject *parent = nullptr);

public slots:
    // Caches a decoded sample in memory
    void cacheSample(const QString& filePath, std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, dtracker::sample::types::SampleMetadata meta);

    // Placeholder for adding samples to the registry
    void addSample(const QString& filePath);

    // Checks if the file is already cached and emits a result
    void isCached(const QString& filePath);

    // Retrieves PCM data from the cache (if available)
    void requestPCMData(const QString& filePath);

signals:
    void sampleAdded(int id, const QString& name);

    // Emitted when a sample is inserted into the cache
    void sampleCached(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData);

    // Emits whether or not a sample exists inside the cache
    void sampleIsCached(const QString& filePath, bool isCached);

    // Emitted when PCMData is found
    void PCMDataFound(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, dtracker::audio::types::AudioProperties properties);

private:
    // Owns the cache and sample registry
    dtracker::sample::Manager m_sampleManager;
};
}

// Allows PCMData and SampleMetadata in Qt signals
Q_DECLARE_METATYPE(dtracker::audio::types::PCMData)
Q_DECLARE_METATYPE(dtracker::sample::types::SampleMetadata)

#endif // SAMPLE_MANAGER_WORKER_H
