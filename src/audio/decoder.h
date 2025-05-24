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

    // Loads the file and emits a signal with decoded PCM data
    Q_INVOKABLE void load(const QString &filePath);

signals:
    // Emitted when decoding is complete and data is ready
    void sampleReady(std::vector<float> pcmData, unsigned int sampleRate);
    void errorOccurred(const QString &message);

private slots:
    void onBufferReady();
    void onFinished();

private:
    QAudioDecoder *m_decoder;
    std::vector<float> m_accumulatedSamples;
    int m_outputChannels = 0;
    int m_sampleRate = 0;
};

} // namespace Dtracker::Audio


#endif // DECODER_H
