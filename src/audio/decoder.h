#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QFileInfo>
#include <QAudioDecoder>
#include <QAudioBuffer>
#include <vector>
#include <dtracker/audio/types.hpp>

namespace Dtracker::Audio {

class Decoder : public QObject
{
    Q_OBJECT

public:
    explicit Decoder(QObject *parent = nullptr);

    // Begin decoding the audio file at the given path
    void startDecodingFile(const QString &filePath);

signals:
    // Emitted when decoding is complete and normalized float PCM data is ready
    void sampleReady(std::shared_ptr<const dtracker::audio::types::PCMData> pcmData, unsigned int sampleRate, unsigned int sampleBitDepth, QFileInfo fileInfo);

    // Emitted if decoding fails (unsupported format, etc.)
    void errorOccurred(const QString &message);

private slots:
    // Triggered when a new decoded audio buffer is available
    void onBufferReady();

    // Triggered when all decoding has finished
    void onFinished();

private:
    QAudioDecoder *m_decoder;                   // Decoding engine
    std::vector<float> m_accumulatedSamples;    // Holds float PCM samples
    int m_outputChannels = 0;                   // Stores the channel count of the current audio file
    unsigned int m_sampleRate = 0;              // Stores the sample rate of the current audio file
    unsigned int m_sampleBitDepth = 0;          // Stores the bit depth of the current audio file
    QFileInfo m_fileInfo;                       // Information on the file being decoded

    QFileInfo m_nextJob;                        // Holds the next decoding job
};

} // namespace Dtracker::Audio

#endif // DECODER_H
