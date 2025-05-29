#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>
#include <dtracker/audio/sample_manager.hpp>
Q_IMPORT_QML_PLUGIN(dtracker_gui_audioPlugin)

int main(int argc, char *argv[])
{
    qRegisterMetaType<dtracker::audio::SampleManager*>("dtracker::audio::SampleManager*");

    // Which audio files can be decoded depends on QT_MEDIA_BACKEND
    qputenv("QT_MEDIA_BACKEND", "ffmpeg");
    qDebug() << "QT_MEDIA_BACKEND:" << qgetenv("QT_MEDIA_BACKEND");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("dtracker_gui", "Main");

    return app.exec();
}
