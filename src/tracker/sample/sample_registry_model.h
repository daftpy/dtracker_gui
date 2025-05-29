#ifndef SAMPLE_REGISTRY_MODEL_H
#define SAMPLE_REGISTRY_MODEL_H

#include <QAbstractListModel>
#include <vector>
#include "sample_entry.h"

class SampleRegistryModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit SampleRegistryModel(QObject *parent = nullptr);

    enum SampleRoles {
        IdRole = Qt::UserRole + 1,
        NameRole
    };

    // Required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void addSample(int id, const QString &name);
    void clear();

private:
    std::vector<Dtracker::Tracker::SampleEntry> m_entries;
};

#endif // SAMPLE_REGISTRY_MODEL_H
