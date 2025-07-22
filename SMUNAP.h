#ifndef SMUNAP_H
#define SMUNAP_H

#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>
#include <math.h>

#define MUXA0 3
#define MUXA1 4
#define MUXA2 5
#define MUXEN 6

extern Adafruit_MCP4725 DAC;
extern Adafruit_ADS1115 ads;

class SMU
{
public:
    String DataPackage;
    float multiplier = 0.0001875;
    float lastVoltageDiff0_1;
    int16_t resultDiff0_1;
    int16_t resultCh2;
    float lastVoltageCh2;
    double lastCurrent;
    float TransimpedanceResistor = 50000;
    int count;

    String dataString = "";

    float LSVStepTime;
    float LSVVoltageStep;
    float LSVInitialVoltage;
    float LSVFinalVoltage;
    
    int CVStepTime;
    float CVVoltageStep;
    float CVInitialVoltage;
    float CVFinalVoltage;
    float CVPeakVoltage;
    float CVPeakVoltage2;
    int CVCycles;
    
    int DPVPulseTime;
    float DPVVoltageStep;
    float DPVInitialVoltage;
    float DPVFinalVoltage;
    float DPVPulseVoltage;
    int DPVLowTime;

    int SWVIntervalTime;
    float SWVVoltageStep;
    float SWVInitialVoltage;
    float SWVFinalVoltage;
    float SWVPulseVoltage;

    void SerialInputInterpretation();
    void LinearSweepVoltammetry();
    void CyclicSweepVoltammetry();
    void SquareWaveVoltammetry();
    void SendData();
    void ReadCurrent();
    float ReadCurrentSWV();
    void ADSSetGain(float voltage);
    void MeanOfMeasures(bool flag);
    void ApplyVoltage(float voltage);
    void ApplyVoltageSWV(float voltage);
};

#endif