#include "sample_manager_worker.h"
#include <QDebug>
#include <QFileInfo>

namespace Dtracker::Tracker::Sample {

// Insert decoded PCM data into the cache and emit result
SampleManagerWorker::SampleManagerWorker(std::shared_ptr<dtracker::sample::Manager> manager, QObject *parent)
    : QObject{parent}, m_sampleManager(std::move(manager))
{

}

void SampleManagerWorker::cacheSample(const QString &filePath,
                                      std::shared_ptr<const dtracker::audio::types::PCMData> pcmData,
                                      dtracker::sample::types::SampleMetadata meta)
{
    qDebug() << "Worker: caching sample";
    auto pcmPtr = m_sampleManager->cacheSample(filePath.toStdString(), std::move(pcmData), meta);

    // Notify listeners that the sample is now cached
    emit sampleCached(std::move(pcmPtr));
}

// Placeholder: not needed for your current setup
void SampleManagerWorker::addSample(const QString &filePath)
{
    // TODO: registry logic (e.g., assign ID and reuse later)
    auto cacheEntry = m_sampleManager->peekCache(filePath.toStdString());

    if (cacheEntry.has_value()) {
        auto id = m_sampleManager->addSample(filePath.toStdString(),std::move(cacheEntry.value().data),
            {});

        qDebug() << "Sample added, assigned id" << id;
        emit sampleAdded(id, QFileInfo(filePath).fileName());
        return;
    }

    // todo: cache miss
    qDebug() << "Sample not found in cache, could not add";
}

// Check if sample is cached in memory and emit result
void SampleManagerWorker::isCached(const QString &filePath)
{
    bool isCached = m_sampleManager->contains(filePath.toStdString());

    qDebug() << "SampleManagerWorker:" << filePath << "is" << (isCached ? "" : "not") << "cached";
    emit sampleIsCached(filePath, isCached);
}

// Retrieve cached PCM data (and metadata) to send to AudioManager
void SampleManagerWorker::requestPCMData(const QString &filePath)
{
    auto cacheEntry = m_sampleManager->peekCache(filePath.toStdString());

    if (cacheEntry.has_value()) {
        qDebug() << "cache entry found";

        emit PCMDataFound(std::move(cacheEntry->data), cacheEntry->properties);
    }
}

} // namespace Dtracker::Tracker::Sample
