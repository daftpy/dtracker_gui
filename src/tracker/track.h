#ifndef TRACK_H
#define TRACK_H

#include <QObject>
#include <QQmlEngine>

class Track : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit Track(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    QString m_name;
};

#endif // TRACK_H
