#include "SMUNAP.h"

void SMU::SerialInputInterpretation()
{
    int commaIndex, commaIndexAux, startIndex, numFloats, lastCommaIndex, index;
    String input = "";
    input = Serial.readString();
    String restOfString = "";
    int Technique;

    index = input.indexOf(',');

    if (index != -1)
    {
        Technique = input.substring(0, index).toInt();

        restOfString = input.substring(index + 1);
    }
    else
    {
        Technique = input.toInt();
        restOfString = "";
    }

    switch (Technique)
    {
    case 0:
        commaIndex = restOfString.indexOf(',');
        LSVStepTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        LSVVoltageStep = (restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001);

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        LSVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        LSVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        LSVStepTime = (LSVVoltageStep * 1000) / LSVStepTime;

        LinearSweepVoltammetry();
        break;
    case 1:
        commaIndex = restOfString.indexOf(',');
        CVStepTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVPeakVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVPeakVoltage2 = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVCycles = restOfString.substring(commaIndexAux + 1, commaIndex).toInt();

        CVStepTime = (CVVoltageStep * 1000) / CVStepTime;

        CyclicSweepVoltammetry();
        break;
    case 2:
        commaIndex = restOfString.indexOf(',');
        DPVPulseTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        DPVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat();

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        DPVPulseVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() * 0.001;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVLowTime = restOfString.substring(commaIndex + 1, commaIndexAux).toInt();

        // Serial.println(DPVPulseTime);
        // Serial.println(DPVVoltageStep);
        // Serial.println(DPVInitialVoltage);
        // Serial.println(DPVFinalVoltage);
        // Serial.println(DPVPulseVoltage);
        // Serial.println(DPVLowTime);
        SquareWaveVoltammetry();
        break;
    default:
        break;
    }
}

void SMU::LinearSweepVoltammetry()
{
    int timer = 0;
    int direction;

    float i = LSVInitialVoltage;

    if (LSVFinalVoltage > LSVInitialVoltage)
        direction = 1;
    else
        direction = -1;

    while ((direction == 1 && i < LSVFinalVoltage) || (direction == -1 && i > LSVFinalVoltage))
    {
        timer = millis();
        ApplyVoltage(i);
        ReadCurrent();

        while ((millis() - timer) < LSVStepTime)
        {
        }
        i += direction * LSVVoltageStep;
    }
    DAC.setVoltage(0, false);
}

void SMU::CyclicSweepVoltammetry()
{
    int contador = 0;
    int timer = 0;

    int direction;
    float i = CVInitialVoltage;

    if (CVPeakVoltage > CVInitialVoltage)
        direction = 1;
    else
        direction = -1;

    for (int cycle = 0; cycle < CVCycles; cycle++)
    {

        while ((direction == 1 && i < CVPeakVoltage) || (direction == -1 && i > CVPeakVoltage))
        {
            timer = millis();
            ApplyVoltage(i);
            ReadCurrent();

            while ((millis() - timer) < LSVStepTime)
            {
            }
            i += direction * CVVoltageStep;
        }

        if (CVPeakVoltage2 > CVPeakVoltage)
            direction = 1;
        else
            direction = -1;

        while ((direction == 1 && i < CVPeakVoltage2) || (direction == -1 && i > CVPeakVoltage2))
        {
            timer = millis();
            ApplyVoltage(i);
            ReadCurrent();

            count++;
            while ((millis() - timer) < LSVStepTime)
            {
            }
            i += direction * CVVoltageStep;
        }

        if (CVFinalVoltage > CVPeakVoltage2)
            direction = 1;
        else
            direction = -1;

        while ((direction == 1 && i < CVFinalVoltage) || (direction == -1 && i > CVFinalVoltage))
        {
            timer = millis();
            ApplyVoltage(i);
            ReadCurrent();

            while ((millis() - timer) < LSVStepTime)
            {
            }
            i += direction * CVVoltageStep;
        }
    }
    DAC.setVoltage(0, false);
}

void SMU::SquareWaveVoltammetry()
{
    DataPackage = "";
    int timer = 0;
    count = 0;
    int direction;

    SWVIntervalTime = 0;
    SWVVoltageStep = 0.005;
    SWVInitialVoltage = 0;
    SWVFinalVoltage = 0.4;
    SWVPulseVoltage = 0.02;

    if (SWVFinalVoltage > SWVInitialVoltage)
        direction = 1;
    else
        direction = -1;

    float lastVoltage = SWVInitialVoltage;
    ApplyVoltage(lastVoltage);
    // ReadCurrent();

    while ((direction == 1 && lastVoltage < SWVFinalVoltage) || (direction == -1 && lastVoltage > SWVFinalVoltage))
    {
        timer = millis();
        lastVoltage += direction * (SWVPulseVoltage + SWVVoltageStep);

        ApplyVoltage(lastVoltage);
        // ReadCurrent();

        lastVoltage -= direction * SWVPulseVoltage;

        ApplyVoltage(lastVoltage);
        // ReadCurrent();

        if (count == 10)
        {
            SendData();
            count = 0;
        }
        count++;

        while ((millis() - timer) < DPVLowTime)
        {
        }
    }
    count++;
    if (count != 0)
        SendData();
    DAC.setVoltage(0, false);
}

void SMU::ApplyVoltage(float voltage)
{
    float DACVoltageBit;
    DACVoltageBit = voltage * (10 / 3.3F) + 1.723 - 0.02;
    DACVoltageBit = DACVoltageBit * (4095 / 5);
    DACVoltageBit = constrain(DACVoltageBit, 0, 4095);

    DAC.setVoltage(DACVoltageBit, false);
    float currentVoltage = (ads.readADC_Differential_0_1() * multiplier) - 0.5 + 0.05;

    dataString = String(voltage, 5);
}

void SMU::SendData()
{
    // Serial.print(DataPackage);
    DataPackage = "";
}

// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V    0.1875mV (default)
// ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V    0.125mV
// ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V    0.0625mV
// ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V    0.03125mV
// ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V    0.015625mV
// ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V    0.0078125mV
void SMU::ReadCurrent()
{
    multiplier = 0.0001875F;
    float corrente = ((ads.readADC_Differential_2_3() * multiplier) - 2.5) / 50000;
    
    dataString = dataString + "," + String(corrente, 9);
    Serial.println(dataString);
}

void SMU::MeanOfMeasures(bool flag)
{
    if (flag)
    {
        resultDiff0_1 = 0;
        for (int i = 0; i < 5; i++)
        {
            resultDiff0_1 = resultDiff0_1 + ads.readADC_Differential_0_1();
        }
        resultDiff0_1 = resultDiff0_1 / 5;
    }
    else
    {
        resultCh2 = 0;
        for (int i = 0; i < 5; i++)
        {
            resultCh2 = resultCh2 + ads.readADC_SingleEnded(2);
        }
        resultCh2 = resultCh2 / 5;
    }
}

void SMU::ADSSetGain(float voltage)
{
    if (abs(voltage) > 3.9)
    {
        ads.setGain(GAIN_TWOTHIRDS);
        multiplier = 0.0001875;
        // Serial.println("GAIN_TWOTHIRDS");
    }
    else if (abs(voltage) < 3.9 && abs(voltage) > 1.9)
    {
        ads.setGain(GAIN_ONE);
        multiplier = 0.000125;
        // Serial.println("GAIN_ONE");
    }
    else if (abs(voltage) < 1.9 && abs(voltage) > 0.85)
    {
        ads.setGain(GAIN_TWO);
        multiplier = 0.0000625;
        // Serial.println("GAIN_TWO");
    }
    else if (abs(voltage) < 0.85 && abs(voltage) > 0.38)
    {
        ads.setGain(GAIN_FOUR);
        multiplier = 0.00003125;
        // Serial.println("GAIN_FOUR");
    }
    else if (abs(voltage) < 0.38 && abs(voltage) > 0.19)
    {
        ads.setGain(GAIN_EIGHT);
        multiplier = 0.000015625;
        // Serial.println("GAIN_EIGHT");
    }
    else if (abs(voltage) < 0.19)
    {
        ads.setGain(GAIN_SIXTEEN);
        multiplier = 0.0000078125;
        // Serial.println("GAIN_SIXTEEN");
    }
}