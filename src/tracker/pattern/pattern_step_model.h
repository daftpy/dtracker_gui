#ifndef PATTERN_STEP_MODEL_H
#define PATTERN_STEP_MODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QList>
#include <QAbstractListModel>

namespace Dtracker::Tracker {

// PatternStepModel represents a simple step sequencer pattern.
// Each step holds a sample ID, or -1 if the step is empty.
class PatternStepModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        SampleIdRole = Qt::UserRole + 1  // Role to access the sample ID of a step
    };
    Q_ENUM(Roles)

    explicit PatternStepModel(QObject* parent = nullptr);

    // Returns the number of steps in the pattern
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    // Returns the data (sample ID) for a specific step and role
    QVariant data(const QModelIndex& index, int role) const override;

    // Sets the sample ID at a given step and emits necessary signals
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    // Provides role names for QML data binding
    QHash<int, QByteArray> roleNames() const override;

    // Replaces the internal list of steps
    void setSteps(const QList<int>& steps);

    // Returns the current list of step sample IDs
    QList<int> steps() const;

    // QML-callable method to set a sample ID at a given step
    Q_INVOKABLE bool setStep(int index, int sampleId);

signals:
    // Emitted when a step is assigned a new sample ID
    void stepSet(int index, int sampleId);

private:
    QList<int> m_steps;  // Internal storage for step sample IDs (-1 means empty)
};

} // namespace Dtracker::Tracker

#endif // PATTERN_STEP_MODEL_H
