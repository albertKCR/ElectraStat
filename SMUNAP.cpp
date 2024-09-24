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

        // Serial.println(LSVStepTime);
        // Serial.println(LSVVoltageStep,5);
        // Serial.println(LSVInitialVoltage);
        // Serial.println(LSVFinalVoltage);

        LinearSweepVoltammetry();
        break;
    case 1:
        commaIndex = restOfString.indexOf(',');
        CVStepTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() + 2.5;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() + 2.5;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        CVPeakVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() + 2.5;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        CVCycles = restOfString.substring(commaIndex + 1, commaIndexAux).toInt();

        CyclicSweepVoltammetry();
        break;
    case 2:
        commaIndex = restOfString.indexOf(',');
        DPVPulseTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        DPVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() + 2.5;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        DPVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() + 2.5;

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
        DifferentialPulseVoltammetry();
        break;
    case 3:
        commaIndex = restOfString.indexOf(',');
        NPVPulseTime = restOfString.substring(0, commaIndex).toInt();

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        NPVVoltageStep = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() * 0.001;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        NPVInitialVoltage = restOfString.substring(commaIndexAux + 1, commaIndex).toFloat() + 2.5;

        commaIndexAux = restOfString.indexOf(',', commaIndex + 1);
        NPVFinalVoltage = restOfString.substring(commaIndex + 1, commaIndexAux).toFloat() + 2.5;

        commaIndex = restOfString.indexOf(',', commaIndexAux + 1);
        NPVLowTime = restOfString.substring(commaIndexAux + 1, commaIndex).toInt();

        // Serial.println(NPVPulseTime);
        // Serial.println(NPVVoltageStep);
        // Serial.println(NPVInitialVoltage);
        // Serial.println(NPVFinalVoltage);
        // Serial.println(NPVLowTime);
        NormalPulseVoltammetry();
        break;
    default:
        break;
    }
}

void SMU::LinearSweepVoltammetry()
{
    DataPackage = "";
    int timer = 0;
    count = 0;

    LSVInitialVoltage = -0.4;
    LSVFinalVoltage = 0.5;
    LSVVoltageStep = 0.005;
    LSVStepTime = 0;

    for (float i = LSVInitialVoltage; i <= LSVFinalVoltage; i = i + LSVVoltageStep)
    {
        ApplyVoltage(i);
        timer = millis();
        while ((millis() - timer) < LSVStepTime)
        {
        }
        ReadCurrent();
        // ReadVoltage();

        if (count == 10)
        {
            // SendData();
            count = 0;
        }
        count++;
    }
    if (count = !0)
        // SendData();
        DAC.setVoltage(0, false);
    Serial.println("OK");
}

void SMU::CyclicSweepVoltammetry()
{
    int contador = 0;
    DataPackage = "";
    int timer = 0;
    count = 0;
    CVVoltageStep = 0.005;
    CVInitialVoltage = 0;
    CVFinalVoltage = 0;
    CVPeakVoltage = 0.5;
    CVPeakVoltage2 = -0.5;

    for (int cycle = 0; cycle < CVCycles; cycle++)
    {
        for (float v = CVInitialVoltage; v <= CVPeakVoltage; v += CVVoltageStep)
        {
            DAC.setVoltage(v, false);
            ReadCurrent();
            timer = millis();
            while ((millis() - timer) < CVStepTime)
            {
            }
            if (count == 10)
            {
                SendData();
                count = 0;
            }
            count++;
        }
        for (float v = CVPeakVoltagebit; v >= CVFinalVoltagebit; v -= CVVoltageStepbit)
        {
            DAC.setVoltage(v, false);
            ReadCurrent();
            timer = millis();
            while ((millis() - timer) < CVStepTime)
            {
            }
            if (count == 10)
            {
                SendData();
                count = 0;
            }
            count++;
        }
    }
    if (count = !0)
        SendData();
    DAC.setVoltage(2047, false);
}

void SMU::DifferentialPulseVoltammetry()
{
    DataPackage = "";
    int timer = 0;
    count = 0;
    int DPVVoltageStepbit = ceil((DPVVoltageStep * 4095) / 5);
    int DPVInitialVoltagebit = (DPVInitialVoltage * 4095) / 5;
    int DPVFinalVoltagebit = (DPVFinalVoltage * 4095) / 5;
    int DPVPulseVoltagebit = (DPVPulseVoltage * 4095) / 5;

    float lastVoltage = DPVInitialVoltagebit;
    DAC.setVoltage(lastVoltage, false);
    ReadCurrent();

    while (lastVoltage < DPVFinalVoltagebit)
    {
        lastVoltage = lastVoltage + DPVPulseVoltagebit;
        timer = millis();
        while ((millis() - timer) < DPVLowTime)
        {
        }
        DAC.setVoltage(lastVoltage, false);
        // Serial.print(lastVoltage*5/4095);
        // Serial.print(",");
        ReadCurrent();

        lastVoltage = lastVoltage - DPVVoltageStepbit;
        timer = millis();
        while ((millis() - timer) < DPVPulseTime)
        {
        }
        DAC.setVoltage(lastVoltage, false);
        // Serial.print(lastVoltage*5/4095);
        // Serial.print(",");
        ReadCurrent();

        if (count == 10)
        {
            SendData();
            count = 0;
        }
        count++;
    }
    DAC.setVoltage(DPVFinalVoltagebit, false);
    ReadCurrent();
    // Serial.print(lastVoltage*5/4095);
    // Serial.print(",");
    count++;
    if (count != 0)
        SendData();
    DAC.setVoltage(2047, false);
}

void SMU::NormalPulseVoltammetry()
{
    DataPackage = "";
    int timer = 0;
    count = 0;
    int NPVVoltageStepbit = ceil((NPVVoltageStep * 4095) / 5);
    int NPVInitialVoltagebit = (NPVInitialVoltage * 4095) / 5;
    int NPVFinalVoltagebit = (NPVFinalVoltage * 4095) / 5;

    float lastVoltage = NPVInitialVoltagebit;
    DAC.setVoltage(NPVInitialVoltagebit, false);
    ReadCurrent();

    while (lastVoltage < NPVFinalVoltagebit)
    {
        lastVoltage = lastVoltage + NPVVoltageStepbit;
        DAC.setVoltage(lastVoltage, false);
        // Serial.print(lastVoltage*5/4095,3);
        // Serial.print(",");
        timer = millis();
        while ((millis() - timer) < NPVPulseTime)
        {
        }
        ReadCurrent();
        count++;

        if (count == 10)
        {
            SendData();
            count = 0;
        }

        DAC.setVoltage(NPVInitialVoltagebit, false);
        // Serial.print(NPVInitialVoltagebit*5/4095,3);
        // Serial.print(",");
        timer = millis();
        while ((millis() - timer) < NPVLowTime)
        {
        }
        ReadCurrent();
        count++;

        if (count == 10)
        {
            SendData();
            count = 0;
        }
    }
    if (count != 0)
        SendData();
    DAC.setVoltage(2047, false);
}

void SMU::VoltageControl(float desiredVoltage, float bitDesiredVoltage)
{
    bool loop = true;
    float currentVoltage;
    float tolerance = 0.0006;
    float dacOutputVoltage = bitDesiredVoltage;
    float Kp = 0.8;
    float Ki = 0;
    int count = 0;

    float currentTime, deltaTime, previousTime;
    float integral;
    previousTime = millis();

    currentVoltage = ads.readADC_Differential_0_1() * multiplier;
    ADSSetGain(currentVoltage);

    while (loop)
    {
        if (count > 5)
            break;
        count++;
        currentTime = millis();
        deltaTime = (currentTime - previousTime) / 1000.0;
        previousTime = currentTime;

        currentVoltage = (ads.readADC_Differential_0_1() * multiplier) - 0.5;
        // Serial.println(currentVoltage, 5);

        float error = desiredVoltage - (currentVoltage);

        // Serial.print("error: ");
        // Serial.println(error, 4);

        currentVoltage = ((currentVoltage + 0.5F) * (3.33333F)) * 819;
        float errorBit = bitDesiredVoltage - currentVoltage;
        // Serial.print("errorBit: ");
        // Serial.println(errorBit);

        integral += errorBit * deltaTime;

        if (abs(error) <= tolerance)
        {
            loop = false;
        }

        float correction = (Kp * errorBit) + (Ki * integral);
        dacOutputVoltage += correction;
        // Serial.print("correction: ");
        // Serial.println(Ki * integral);

        // Serial.print("dacOutputVoltage: ");
        // Serial.println(dacOutputVoltage);
        DAC.setVoltage(dacOutputVoltage, false);
    }
    ads.setGain(GAIN_TWOTHIRDS);
    multiplier = 0.0001875;
    currentVoltage = (ads.readADC_Differential_0_1() * multiplier) - 0.5 + 0.05;
    // Serial.print(currentVoltage, 5);
    // Serial.print(";");
    // Serial.println("OK");
    Serial.print(desiredVoltage, 5);
    Serial.print(";");
    Serial.println(currentVoltage, 5);
}

void SMU::ApplyVoltage(float voltage)
{
    ads.setGain(GAIN_TWOTHIRDS);
    float DACVoltageBit;
    DACVoltageBit = voltage * (10 / 3.3F) + 1.723 - 0.02;
    // Serial.print("tensao: ");
    // Serial.println(DACVoltageBit);
    DACVoltageBit = DACVoltageBit * (4095 / 5);
    DACVoltageBit = constrain(DACVoltageBit, 0, 4095);

    // Serial.print("DACVoltageBit: ");
    // Serial.println(DACVoltageBit);

    DAC.setVoltage(DACVoltageBit, false);
    float currentVoltage = (ads.readADC_Differential_0_1() * multiplier) - 0.5 + 0.05;
    DataPackage = DataPackage + String(currentVoltage, 4) + ",";
    Serial.print(voltage, 5);
    Serial.print(";");
    // Serial.println(currentVoltage, 5);

    // VoltageControl(voltage, DACVoltageBit);
}

void SMU::SendData()
{
    Serial.print(DataPackage);
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
    ads.setGain(GAIN_TWOTHIRDS);
    multiplier = 0.0001875F;
    // resultCh2 = ads.readADC_SingleEnded(2);
    //  resultCh2 = ads.readADC_Differential_2_3();
    // ADSSetGain(resultCh2 * multiplier);
    //  MeanOfMeasures(false);
    //  lastVoltageCh2 = (resultCh2 * multiplier);
    //  lastCurrent = lastVoltageCh2 / TransimpedanceResistor;
    // DataPackage = DataPackage + String((lastCurrent - 2.5), 9) + ",";
    float tensao = ((ads.readADC_Differential_2_3() * multiplier) - 2.5) / 50000;
    Serial.println(tensao, 9);
    // DataPackage = DataPackage + String(tensao, 9) + ",";
    ads.setGain(GAIN_TWOTHIRDS);
}

void SMU::ReadVoltage()
{
    resultDiff0_1 = ads.readADC_Differential_0_1();
    // ADSSetGain(resultDiff0_1);
    // MeanOfMeasures(true);
    lastVoltageCh2 = (resultDiff0_1 * multiplier);
    DataPackage = DataPackage + String((lastVoltageCh2 - 0.5), 4) + ",";

    // Serial.println(lastVoltageCh2 - 0.5, 5);

    ads.setGain(GAIN_TWOTHIRDS);
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