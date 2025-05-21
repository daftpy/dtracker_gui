#include "audiodeviceinfo.h"

AudioDeviceInfo::AudioDeviceInfo()
    : name("Invalid"), outputChannels(0), inputChannels(0),
    duplexChannels(0), isDefaultOutput(false), isDefaultInput(false),
    preferredSampleRate(0)
{}

AudioDeviceInfo::AudioDeviceInfo(const RtAudio::DeviceInfo &info)
    : name(QString::fromStdString(info.name)),
    outputChannels(info.outputChannels),
    inputChannels(info.inputChannels),
    duplexChannels(info.duplexChannels),
    isDefaultOutput(info.isDefaultOutput),
    isDefaultInput(info.isDefaultInput),
    preferredSampleRate(info.preferredSampleRate)
{
    // Convert std::vector<unsigned int> to QList<unsigned int>
    for (unsigned int rate : info.sampleRates) {
        sampleRates.append(rate);
    }
}
