#include "decoder.h"
#include <QDebug>
#include <QUrl>
#include <QAudioFormat>

namespace Dtracker::Audio {

Decoder::Decoder(QObject *parent)
    : QObject(parent), m_decoder(new QAudioDecoder(this))
{
    // Connect bufferReady to process incoming decoded audio chunks
    connect(m_decoder, &QAudioDecoder::bufferReady, this, &Decoder::onBufferReady);

    // Connect finished to handle the end of decoding
    connect(m_decoder, &QAudioDecoder::finished, this, &Decoder::onFinished);

    // Connect error signal to report decoding failures
    connect(m_decoder, QOverload<QAudioDecoder::Error>::of(&QAudioDecoder::error),
            this, [this](QAudioDecoder::Error error) {
                qDebug() << "Decoder error:" << error << m_decoder->errorString();
                emit errorOccurred(m_decoder->errorString());
            });

    // Log the media backend for debugging purposes
    qDebug() << "QT_MEDIA_BACKEND:" << qgetenv("QT_MEDIA_BACKEND");
}

void Decoder::load(const QString &filePath)
{
    // Clear any previously decoded samples
    m_accumulatedSamples.clear();
    m_outputChannels = 0;
    m_sampleRate = 0;

    // Log file path and decoder format info
    qDebug() << "FileLoader: setSource path:" << filePath;
    qDebug() << "Decoder format valid:" << m_decoder->audioFormat().isValid();
    qDebug() << "Decoder format:" << m_decoder->audioFormat();

    // Set the source and begin decoding asynchronously
    m_decoder->setSource(QUrl(filePath));
    qDebug() << "FileLoader: decoding...";
    m_decoder->start();
}

void Decoder::onBufferReady()
{
    // Pull the next available decoded audio buffer from the decoder
    QAudioBuffer buffer = m_decoder->read();

    // Cache output channel count and sample rate once from the first buffer
    if (m_outputChannels == 0)
        m_outputChannels = buffer.format().channelCount();
    if (m_sampleRate == 0)
        m_sampleRate = buffer.format().sampleRate();

    // Total number of samples in this buffer (not bytes, just sample count)
    const int sampleCount = buffer.sampleCount();

    // ---- CASE 1: Handle 32-bit float PCM data ----
    if (buffer.format().sampleFormat() == QAudioFormat::Float) {
        const float *data = buffer.constData<float>(); // Already normalized
        m_accumulatedSamples.insert(m_accumulatedSamples.end(), data, data + sampleCount);

    // ---- CASE 2: Handle 16-bit signed integer PCM data ----
    } else if (buffer.format().sampleFormat() == QAudioFormat::Int16) {
        const qint16 *data = buffer.constData<qint16>();
        for (int i = 0; i < sampleCount; ++i)
            m_accumulatedSamples.push_back(static_cast<float>(data[i]) / 32768.0f); // Normalize

    // ---- CASE 3: Unsupported format ----
    } else {
        emit errorOccurred("Unsupported sample format (only float and int16 supported)");
        m_decoder->stop();
        return;
    }
}

void Decoder::onFinished()
{
    // Notify QML that decoding is complete and deliver the final float PCM
    qDebug() << "FileLoader: decoding finished...";
    emit sampleReady(std::move(m_accumulatedSamples), static_cast<unsigned int>(m_sampleRate));
}

} // namespace Dtracker::Audio
