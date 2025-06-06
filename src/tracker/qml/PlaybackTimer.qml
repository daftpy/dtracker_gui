import QtQuick

Timer {
    id: patternPlayer
    interval: 100  // 100ms per step = ~10 steps/sec
    repeat: true
    running: false

    property int currentStep: 0
    property PatternStepModel patternModel: null
    property var audioManager: null  // assume it has `playSampleById(int)`

    onTriggered: {
        if (!patternModel)
            return;

        const sampleId = patternModel.data(patternModel.index(currentStep, 0), PatternStepModel.SampleIdRole)
        if (sampleId >= 0) {
            audioManager.playSampleById(sampleId);
        }

        currentStep = (currentStep + 1) % patternModel.rowCount();
    }
}
