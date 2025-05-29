#include "sample_registry.h"

namespace Dtracker::Tracker::Sample {

SampleRegistry::SampleRegistry(QObject *parent)
    : QObject(parent)
{
}

void SampleRegistry::setSampleManager(dtracker::audio::SampleManager* manager)
{
    if (m_sampleManager == manager)
        return;

    m_sampleManager = manager;
    emit sampleManagerChanged();
}

void SampleRegistry::addSample(const QString &name, std::vector<float> pcm, unsigned int rate)
{
    if (!m_sampleManager)
        return;

    // Add to backend manager and get ID
    int id = m_sampleManager->addSample(std::move(pcm), rate);

    // Add to the view model
    m_model->addSample(id, name);

    // Notify QML (optional)
    emit sampleAdded(id);
}

void SampleRegistry::removeSample(int id)
{
    if (!m_sampleManager)
        return;

    if (m_sampleManager->removeSample(id))
    {
        qDebug() << "Sample removed from sampleManager";
        m_model->removeSample(id);
    }

    emit sampleRemoved(id);
}

}
