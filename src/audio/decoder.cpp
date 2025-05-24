#include "decoder.h"
#include <QDebug>
#include <QUrl>
#include <QAudioFormat>

namespace Dtracker::Audio {

Decoder::Decoder(QObject *parent)
    : QObject(parent), m_decoder(new QAudioDecoder(this))
{
    connect(m_decoder, &QAudioDecoder::bufferReady, this, &Decoder::onBufferReady);
    connect(m_decoder, &QAudioDecoder::finished, this, &Decoder::onFinished);
    connect(m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
            this, [this](QAudioDecoder::Error error) {
                qDebug() << "Decoder error:" << error << m_decoder->errorString();
                emit errorOccurred(m_decoder->errorString());
            });
    qDebug() << "QT_MEDIA_BACKEND:" << qgetenv("QT_MEDIA_BACKEND");

}

void Decoder::load(const QString &filePath)
{
    m_accumulatedSamples.clear();
    m_outputChannels = 0;
    m_sampleRate = 0;

    qDebug() << "FileLoader: setSource path:" << filePath;
    qDebug() << "Decoder format valid:" << m_decoder->audioFormat().isValid();
    qDebug() << "Decoder format:" << m_decoder->audioFormat();

    m_decoder->setSource(QUrl(filePath));
    qDebug() << "FileLoader: decoding...";
    m_decoder->start();
}

void Decoder::onBufferReady()
{
    // Pulls the next available decoded audio buffer from the decoder.
    QAudioBuffer buffer = m_decoder->read();

    // Store channel count and sample rate once (on first buffer)
    if (m_outputChannels == 0)
        m_outputChannels = buffer.format().channelCount();
    if (m_sampleRate == 0)
        m_sampleRate = buffer.format().sampleRate();

    // Total number of audio samples in this buffer (not bytes)
    // For stereo, this is 2 samples per frame.
    const int sampleCount = buffer.sampleCount();

    // ---- CASE 1: Buffer contains 32-bit float PCM samples ----
    if (buffer.format().sampleFormat() == QAudioFormat::Float) {
        // Access the raw float data (samples are already in [-1.0, 1.0] range)
        const float *data = buffer.constData<float>();

        // Append them directly to the accumulated PCM data
        m_accumulatedSamples.insert(m_accumulatedSamples.end(), data, data + sampleCount);

        // ---- CASE 2: Buffer contains 16-bit signed integer PCM samples ----
    } else if (buffer.format().sampleFormat() == QAudioFormat::Int16) {
        // Access the raw int16 data
        const qint16 *data = buffer.constData<qint16>();

        // Convert each int16 sample to float in range [-1.0, 1.0]
        for (int i = 0; i < sampleCount; ++i)
            m_accumulatedSamples.push_back(static_cast<float>(data[i]) / 32768.0f);

        // ---- CASE 3: Unsupported audio sample format ----
    } else {
        // Stop decoding and notify QML
        emit errorOccurred("Unsupported sample format (only float and int16 supported)");
        m_decoder->stop();
        return;
    }
}


void Decoder::onFinished()
{
    qDebug() << "FileLoader: decoding finished...";
    emit sampleReady(std::move(m_accumulatedSamples), static_cast<unsigned int>(m_sampleRate));
}

} // namespace Dtracker::Audio
