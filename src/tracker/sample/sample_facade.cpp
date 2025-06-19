#include "sample_facade.h"

namespace Dtracker::Tracker::Sample {

SampleFacade::SampleFacade(QObject *parent)
    : QObject{parent}
{
    // Create the central, shared sample data manager.
    m_sampleManager = std::make_shared<dtracker::sample::Manager>();

    // Set up a worker on a background thread to handle all sample operations.
    m_workerThread = new QThread(this);
    m_managerWorker = new SampleManagerWorker(m_sampleManager);
    m_managerWorker->moveToThread(m_workerThread);

    // Ensure worker is deleted cleanly when the thread exits.
    connect(m_workerThread, &QThread::finished, m_managerWorker, &QObject::deleteLater);

    // --- Wire up the asynchronous workflows between the facade and the worker ---

    // Delegate cache check requests to the worker.
    connect(this, &SampleFacade::checkCache,
            m_managerWorker, &SampleManagerWorker::isCached);

    // Process the result of the asynchronous cache check.
    connect(m_managerWorker, &SampleManagerWorker::sampleIsCached,
            this, &SampleFacade::handleSampleIsCached);

    // Delegate requests for cached PCM data to the worker.
    connect(this, &SampleFacade::requestPCMData,
            m_managerWorker, &SampleManagerWorker::requestPCMData);

    // Wire up the sample registration workflow.
    connect(this, &SampleFacade::addSample, m_managerWorker, &SampleManagerWorker::addSample);
    connect(m_managerWorker, &SampleManagerWorker::sampleAdded, this, &SampleFacade::handleSampleAdded);

    // For debugging: log when the worker successfully caches a sample.
    connect(m_managerWorker, &SampleManagerWorker::sampleCached,
            this, [](const std::shared_ptr<const dtracker::audio::types::PCMData>& pcm) {
                qDebug() << "Sample successfully cached. Size:" << pcm->size();
            });

    connect(this, &SampleFacade::retrieveSample, m_managerWorker, &SampleManagerWorker::handleRetrieveSample);
    connect (m_managerWorker, &SampleManagerWorker::sampleFound, this, &SampleFacade::handleSampleIsFound);

    // Start the worker thread's event loop.
    m_workerThread->start();
}

SampleFacade::~SampleFacade()
{
    m_workerThread->quit();

    // A short timeout is added as a safeguard in case the thread is stuck
    if (!m_workerThread->wait(3000)) // Wait for up to 3 seconds
    {
        // If the thread did not shut down gracefully, forcefully terminate it
        qDebug() << "Warning: Worker thread did not shut down in time. Forcing termination.";
        m_workerThread->terminate();
        m_workerThread->wait(); // Wait for the termination to complete.
    }
}

// Injects the AudioManager and connects the necessary cross-system signals.
void SampleFacade::setAudioManager(Audio::Manager *manager)
{
    if (m_audioManager != manager) {
        m_audioManager = manager;

        // When a file is decoded, tell the worker to cache the data.
        connect(m_audioManager, &Audio::Manager::fileDecoded,
            m_managerWorker, &SampleManagerWorker::cacheSample);

        // When cached data is found, tell the audio manager to play it.
        connect(m_managerWorker, &SampleManagerWorker::PCMDataFound,
            m_audioManager, &Audio::Manager::previewPCMData);

        connect(m_managerWorker, &SampleManagerWorker::cacheMiss,
                m_audioManager, &Audio::Manager::startDecodingFile);

        qDebug() << "audioManager property updated";

        emit audioManagerChanged();
    }
}

dtracker::sample::Manager *SampleFacade::manager()
{
    return m_sampleManager.get();
}

// Public API to begin the asynchronous sample registration process.
void SampleFacade::registerSample(const QString &filePath)
{
    qDebug() << "Adding sample" << filePath;
    emit addSample(filePath);
}

// Kicks off the async preview workflow by first checking the cache.
void SampleFacade::previewSample(const QString &filePath)
{
    emit checkCache(filePath);
}

void SampleFacade::previewSample(int id)
{
    emit retrieveSample(id);
}

// Slot that continues the preview workflow after the cache check completes.
void SampleFacade::handleSampleIsCached(const QString &filePath, bool isCached)
{
    if (m_audioManager != nullptr && !isCached)
    {
        // Cache miss: Tell the AudioManager to decode the file. This will trigger
        // the worker's cacheSample slot after decoding.
        m_audioManager->startDecoding(filePath);
        return;
    }

    // Cache hit: Request the PCM data from the worker for immediate playback.
    qDebug() << "Sample is already cached, requesting PCM data.";
    emit requestPCMData(filePath);
}

// Slot to update the QML model when the worker confirms a sample was registered.
void SampleFacade::handleSampleAdded(int id, const QString& filePath)
{
    m_sampleRegistry->addSample(id, filePath);
}

void SampleFacade::handleSampleIsFound(dtracker::sample::types::SampleDescriptor descriptor)
{
    // OLD TODO: REFACTORING AWAY
    // auto unit = dtracker::audio::playback::makePlaybackUnit(std::move(descriptor));

    // Release the ownership or crash!
    // emit playbackSample(unit.release());

    emit playbackSampleDescriptor(std::move(descriptor));
}

Audio::Manager* SampleFacade::audioManager() const
{
    return m_audioManager;
}

} // namespace Dtracker::Tracker::Sample
