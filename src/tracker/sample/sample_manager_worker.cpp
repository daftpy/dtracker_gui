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
    qDebug() << "Worker: caching sample for" << filePath;
    auto pcmPtr = m_sampleManager->cacheSample(filePath.toStdString(), std::move(pcmData), meta);
    emit sampleCached(std::move(pcmPtr)); // Notify that caching is done.

    // Check if this newly cached sample was one we were waiting to register.
    if (m_pendingRegistrations.contains(filePath))
    {
        qDebug() << "Worker: Continuing pending registration for" << filePath;

        // The data is now cached, so we can call addSample again to finish the job.
        // This time it will be a cache hit.
        addSample(filePath);

        // Clean up the pending request.
        m_pendingRegistrations.remove(filePath);
    }
}

// Placeholder: not needed for your current setup
void SampleManagerWorker::addSample(const QString &filePath)
{
    // Check if the sample's data is already in the cache.
    if (m_sampleManager->peekCache(filePath.toStdString()).has_value())
    {
        // The manager returns an int, with -1 indicating failure.
        int newId = m_sampleManager->addSample(filePath.toStdString());

        if (newId != -1)
        {
            qDebug() << "Sample registered from existing cache, assigned id" << newId;
            emit sampleAdded(newId, QFileInfo(filePath).fileName());
        }
        else
        {
            qDebug() << "Failed to register sample even though it was found in cache:" << filePath;
        }
    }
    else
    {
        // The data is not in the cache. Request decoding.
        qDebug() << "Sample not in cache. Requesting decoding for" << filePath;

        // Mark this path as pending registration.
        m_pendingRegistrations.insert(filePath);

        emit cacheMiss(filePath);
    }
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

void SampleManagerWorker::handleRetrieveSample(int id)
{
    auto descriptor = m_sampleManager->getSample(id);

    if (descriptor.has_value()) {
        emit sampleFound(std::move(descriptor.value()));
    }
}

} // namespace Dtracker::Tracker::Sample
