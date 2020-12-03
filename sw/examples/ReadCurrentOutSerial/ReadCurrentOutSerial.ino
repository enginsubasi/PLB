/**
  ******************************************************************************
  *
  * @file:      ReadCurrentOutSerial
  * @author:    Engin Subasi
  * @e-mail:    enginsubasi@gmail.com
  * @address:   github.com/enginsubasi/PLB
  *
  * @version:   v 0.0.1
  * @cdate:     03/12/2020
  * @mdate:     03/12/2020
  * @history:   03/12/2020 Created
  *
  * @about:     Read current out serial
  * @device:    Generic Arduino boards
  *
  * @content:
  *     FUNCTIONS:
  *         adcToCurrent    : Returns current value in milliamps
  *         
  * @notes:
  *
  ******************************************************************************
  */

const double rshunt = 0.050;  // Ohm
const double ri     = 1100;   // Ohm
const double rf     = 22000;   // Ohm
double gain = 0;

uint32_t sensorValue = 0;
double cellVoltageValue = 0, cellVoltageValueFilt = 0;
double currentValue = 0, currentValueFilt = 0;

const uint32_t adcVoltageRef = 5;
const uint32_t adcUpValue = 1023;
const int analogInPin = A0;  // Analog input pin that the SLB output is attached to
const int analogInPinForVoltage = A1;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);

  gain = 1 + ( rf / ri );

  Serial.print("Current ");
  Serial.println("Voltage ");
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  cellVoltageValue = analogRead(analogInPinForVoltage);

  currentValue = adcToCurrentMilliamp ( sensorValue );

  currentValueFilt = ( currentValueFilt * 0.95 ) + ( currentValue * 0.05 ); // Exponential moving average filter

  // print the results to the Serial Monitor:
  //Serial.print("Current ");
  Serial.print(currentValueFilt);
  Serial.print(" ");

  // Voltage side
  cellVoltageValue = adcToCellVoltage ( cellVoltageValue );

  cellVoltageValueFilt = ( cellVoltageValueFilt * 0.95 ) + ( cellVoltageValue * 0.05 ); // Exponential moving average filter
  
  //Serial.print("Voltage ");
  Serial.println(cellVoltageValueFilt);

  // wait 10 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(10);
}

double adcToCurrentMilliamp ( uint32_t adcValue )
{
  // This calculation was not simplified for easy understanding.
  double current = 0;

  current = adcValue; /* to cast operation in double domain */

  current = ( ( current * adcVoltageRef ) / adcUpValue );

  current /= gain;

  current /= rshunt;

  return ( current );
}

double adcToCellVoltage ( uint32_t adcValue )
{
  double voltage = 0;

  voltage = adcValue;

  voltage = ( 6.0 * 5.0 * adcValue ) / 1023.0;

  return ( voltage );
}
