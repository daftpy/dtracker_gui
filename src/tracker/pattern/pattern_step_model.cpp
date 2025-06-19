#include "pattern_step_model.h"

namespace Dtracker::Tracker {

// Constructor initializes the model with 16 empty steps (-1 = no sample)
PatternStepModel::PatternStepModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_steps.fill(-1, 16);  // 16 steps initialized to -1 (empty)
}

// Returns the number of steps in the pattern
int PatternStepModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_steps.size();
}

// Returns data for a specific step index and role
QVariant PatternStepModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_steps.size())
        return QVariant();

    if (role == SampleIdRole)
        return m_steps.at(index.row());  // Return sample ID at step

    return QVariant();
}

// Updates the data for a specific index and role
bool PatternStepModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_steps.size())
        return false;

    if (role == SampleIdRole) {
        // Update internal step with new sample ID
        m_steps[index.row()] = value.toInt();

        qDebug() << "Row:" << index.row() << "set to" << SampleIdRole << value.toInt();
        // Notify any attached views that the data has changed
        emit dataChanged(index, index, {SampleIdRole});

        // Emit the step and value
        emit stepSet(index.row(), value.toInt());

        return true;
    }

    return false;  // Unsupported role
}

QHash<int, QByteArray> PatternStepModel::roleNames() const
{
    return {
        {SampleIdRole, "sampleId"}
    };
}

// Replace the entire list of steps (for loading patterns)
void PatternStepModel::setSteps(const QList<int>& steps)
{
    beginResetModel();     // Notifies views to prepare for data reset
    m_steps = steps;       // Replace internal data
    endResetModel();       // Notifies views that reset is complete
}

// Returns the current list of step sample IDs
QList<int> PatternStepModel::steps() const
{
    return m_steps;
}

// Sets a step's sample ID using an index + role
bool PatternStepModel::setStep(int index, int sampleId)
{
    return setData(this->index(index), sampleId, SampleIdRole);
}

} // namespace Dtracker::Tracker
