#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include <QAbstractListModel>

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TrackListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // TRACKLISTMODEL_H
