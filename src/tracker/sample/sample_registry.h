#ifndef SAMPLE_REGISTRY_H
#define SAMPLE_REGISTRY_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <dtracker/audio/sample_manager.hpp>
#include <vector>
#include "sample_registry_model.h"

namespace Dtracker::Tracker::Sample {
// Exposes a list of registered samples and links to the audio sample manager
class SampleRegistry : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Exposes the SampleManager pointer to QML, intended to be set by the audiomanager.sampleManager()
    Q_PROPERTY(dtracker::audio::SampleManager* sampleManager READ sampleManager WRITE setSampleManager NOTIFY sampleManagerChanged)

    // Exposes the list model (used for displaying samples in QML)
    Q_PROPERTY(SampleRegistryModel* model READ model CONSTANT)

public:
    explicit SampleRegistry(QObject *parent = nullptr);

    dtracker::audio::SampleManager* sampleManager() const { return m_sampleManager; }
    void setSampleManager(dtracker::audio::SampleManager* manager);

    // Returns the internal model for sample listing
    SampleRegistryModel* model() const { return m_model; }

    // Adds a sample to the manager and updates the model
    Q_INVOKABLE void addSample(const QString& name, std::vector<float> pcm, unsigned int rate);

    // Removes a sample by ID and updates the model
    Q_INVOKABLE void removeSample(int id);

signals:
    void sampleManagerChanged();  // Emitted when the sample manager is changed
    void sampleAdded(int id);     // Emitted when a sample is added
    void sampleRemoved(int id);   // Emitted when a sample is removed

private:
    dtracker::audio::SampleManager* m_sampleManager = nullptr;  // Pointer to the audio sample manager
    SampleRegistryModel* m_model{new SampleRegistryModel(this)}; // List model containing sample metadata
};
}

// Registers the SampleManager pointer type with Qt's meta-object system
Q_DECLARE_METATYPE(dtracker::audio::SampleManager*)

#endif // SAMPLE_REGISTRY_H
