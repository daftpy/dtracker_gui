#ifndef SAMPLE_REGISTRY_H
#define SAMPLE_REGISTRY_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <dtracker/audio/sample_manager.hpp>
#include <vector>
#include "sample_registry_model.h"

namespace Dtracker::Tracker::Sample {
    class SampleRegistry : public QObject
    {
        Q_OBJECT
        QML_ELEMENT

        Q_PROPERTY(dtracker::audio::SampleManager* sampleManager READ sampleManager WRITE setSampleManager NOTIFY sampleManagerChanged)
        Q_PROPERTY(SampleRegistryModel* model READ model CONSTANT)

    public:
        explicit SampleRegistry(QObject *parent = nullptr);

        dtracker::audio::SampleManager* sampleManager() const { return m_sampleManager; }
        void setSampleManager(dtracker::audio::SampleManager* manager);

        SampleRegistryModel* model() const { return m_model; } // exposes the registry listmodel to qml

        Q_INVOKABLE void addSample(const QString& name, std::vector<float> pcm, unsigned int rate);

    signals:
        void sampleManagerChanged();
        void sampleAdded(int id);

    private:
        dtracker::audio::SampleManager* m_sampleManager = nullptr;
        SampleRegistryModel* m_model{new SampleRegistryModel(this)};
    };
}

Q_DECLARE_METATYPE(dtracker::audio::SampleManager*)

#endif // SAMPLE_REGISTRY_H
