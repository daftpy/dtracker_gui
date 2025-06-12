#include "sample_facade.h"

namespace Dtracker::Tracker::Sample {

SampleFacade::SampleFacade(QObject *parent)
    : QObject{parent}
{
    // Create a background thread and move the worker into it
    m_workerThread = new QThread(this);
    m_managerWorker = new SampleManagerWorker();
    m_managerWorker->moveToThread(m_workerThread);

    // Automatically delete the worker when the thread is finished
    connect(m_workerThread, &QThread::finished, m_managerWorker, &QObject::deleteLater);

    // Forward request to check whether sample is cached
    connect(this, &SampleFacade::checkCache,
            m_managerWorker, &SampleManagerWorker::isCached);

    // Handle the result of that check
    connect(m_managerWorker, &SampleManagerWorker::sampleIsCached,
            this, &SampleFacade::handleSampleIsCached);

    // Log when a sample is successfully cached
    connect(m_managerWorker, &SampleManagerWorker::sampleCached,
            this, [](const std::shared_ptr<const dtracker::audio::types::PCMData>& pcm) {
                qDebug() << "Sample successfully cached. Size:" << pcm->size();
            });

    // Request PCM data for playback from cache
    connect(this, &SampleFacade::requestPCMData,
            m_managerWorker, &SampleManagerWorker::requestPCMData);

    connect(this, &SampleFacade::addSample, m_managerWorker, &SampleManagerWorker::addSample);

    // Start the worker thread
    m_workerThread->start();
}

void SampleFacade::setAudioManager(Audio::Manager *manager)
{
    if (m_audioManager != manager) {
        m_audioManager = manager;

        // When a file is decoded by the AudioManager, cache it
        connect(m_audioManager, &Audio::Manager::fileDecoded,
                m_managerWorker, &SampleManagerWorker::cacheSample);

        // When PCM data is retrieved from cache, forward to AudioManager for playback
        connect(m_managerWorker, &SampleManagerWorker::PCMDataFound,
                m_audioManager, &Audio::Manager::previewPCMData);

        qDebug() << "audioManager property updated";

        emit audioManagerChanged();
    }
}

void SampleFacade::registerSample(const QString &filePath)
{
    qDebug() << "Adding sample" << filePath;
    emit addSample(filePath);
}

void SampleFacade::previewSample(const QString &filePath)
{
    // Step 1: Ask the worker if this sample is already cached
    emit checkCache(filePath);
}

void SampleFacade::handleSampleIsCached(const QString &filePath, bool isCached)
{
    if (m_audioManager != nullptr && !isCached)
    {
        // Step 2A: Not cached — ask AudioManager to decode the sample (which will trigger caching & playback)
        m_audioManager->startDecoding(filePath);
        return;
    }

    qDebug() << "Sample is already cached";

    // Step 2B: Already cached. Ask worker to fetch PCM so AudioManager can play it
    emit requestPCMData(filePath);
}

Audio::Manager* SampleFacade::audioManager() const
{
    return m_audioManager;
}

} // namespace Dtracker::Tracker::Sample
