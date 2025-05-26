#include "filetreemodel.h"
#include <QStandardPaths>

namespace Dtracker::File {

FileTreeModel::FileTreeModel(QObject *parent)
{
    // Show all files and folders except "." and ".."
    setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);

    // Restrict to audio files (add more extensions if needed)
    setNameFilters({ "*.wav", "*.aac" });
    setNameFilterDisables(false);  // Hide files that don't match the filter

    // Initialize to a starting directory (fallback to default if needed)
    setInitialDirectory("C:/");
}

// Only show one column (e.g., the file/folder name)
int FileTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

// Return the currently set root index (used by TreeView)
QModelIndex FileTreeModel::rootIndex() const
{
    return m_rootIndex;
}

// Set the root index and notify if changed
void FileTreeModel::setRootIndex(const QModelIndex index)
{
    if (index == m_rootIndex)
        return;

    m_rootIndex = index;
    emit rootIndexChanged();
}

// Set up the initial directory to browse from
void FileTreeModel::setInitialDirectory(const QString &path)
{
    QDir dir(path);

    // Resolve absolute path, fallback to default if invalid
    if (dir.makeAbsolute())
        setRootPath(dir.path());
    else
        setRootPath(getDefaultRootDir());

    // Set QFileSystemModel’s root index accordingly
    setRootIndex(QFileSystemModel::index(dir.path(), 0));
}

// Retrieve basic file metadata (used in QML for preview and type checking)
QVariantMap FileTreeModel::fileInfoFromIndex(const QModelIndex &index) const
{
    QVariantMap result;

    if (!index.isValid())
        return result;

    result["path"] = filePath(index);
    result["isDir"] = isDir(index);
    result["fileName"] = fileName(index);
    return result;
}

// Provide a default directory (in this case, Downloads folder)
QString FileTreeModel::getDefaultRootDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
}

}
