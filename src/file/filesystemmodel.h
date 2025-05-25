#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include <QObject>
#include <QQmlEngine>
#include <QFileSystemModel>

namespace Dtracker::File {

// Custom file system model exposing filtered file navigation and metadata to QML
class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(FileSystemModel)

    // Expose rootIndex as a QML property to control tree view roots
    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex NOTIFY rootIndexChanged)

public:
    explicit FileSystemModel(QObject *parent = nullptr);

    // Override to restrict to one column (file name)
    int columnCount(const QModelIndex &parent) const override;

    // Getter/setter for the current root index of the file system view
    QModelIndex rootIndex() const;
    void setRootIndex(const QModelIndex index);

    // Initializes the file system model to the given or default directory
    void setInitialDirectory(const QString &path = getDefaultRootDir());

    // Fetches basic file info as a QVariantMap
    Q_INVOKABLE QVariantMap fileInfoFromIndex(const QModelIndex &index) const;

    // Returns a default starting directory
    static QString getDefaultRootDir();

signals:
    // Notifies QML when root index changes
    void rootIndexChanged();

private:
    // Internal state to track the current root index
    QModelIndex m_rootIndex;
};

}

#endif // FILESYSTEMMODEL_H
