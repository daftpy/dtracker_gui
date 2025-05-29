#include "sample_registry_model.h"

SampleRegistryModel::SampleRegistryModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int SampleRegistryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_entries.size();
}

QVariant SampleRegistryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_entries.size()))
        return QVariant();

    const auto &entry = m_entries.at(index.row());

    switch (role) {
    case IdRole:
        return entry.id;
    case NameRole:
        return entry.name;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SampleRegistryModel::roleNames() const
{
    return {
        {IdRole, "id"},
        {NameRole, "name"}
    };
}

void SampleRegistryModel::addSample(int id, const QString &name)
{
    beginInsertRows(QModelIndex(), static_cast<int>(m_entries.size()), static_cast<int>(m_entries.size()));
    m_entries.push_back(Dtracker::Tracker::SampleEntry{id, name});
    endInsertRows();
}

void SampleRegistryModel::removeSample(int id)
{
    for (int row = 0; row < m_entries.size(); ++row) {
        if (m_entries[row].id == id) {
            beginRemoveRows(QModelIndex(), row, row);
            m_entries.erase(m_entries.begin() + row);
            endRemoveRows();
            return;
        }
    }
}

void SampleRegistryModel::clear()
{
    beginResetModel();
    m_entries.clear();
    endResetModel();
}
