#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <vector>

namespace Dtracker::Audio {

class Decoder : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AudioDecoder)

public:
    explicit Decoder(QObject *parent = nullptr);

    // Begin decoding the audio file at the given path
    Q_INVOKABLE void load(const QString &filePath);

signals:
    // Emitted when decoding is complete and normalized float PCM data is ready
    void sampleReady(std::vector<float> pcmData, unsigned int sampleRate, QString sampleName);

    // Emitted if decoding fails (unsupported format, etc.)
    void errorOccurred(const QString &message);

private slots:
    // Triggered when a new decoded audio buffer is available
    void onBufferReady();

    // Triggered when all decoding has finished
    void onFinished();

private:
    QAudioDecoder *m_decoder;               // Decoding engine
    std::vector<float> m_accumulatedSamples; // Holds float PCM samples
    int m_outputChannels = 0;               // Stores the number of channels
    int m_sampleRate = 0;                   // Needed to pass correct timing to audio playback
    QString m_sampleName;
};

} // namespace Dtracker::Audio

#endif // DECODER_H
