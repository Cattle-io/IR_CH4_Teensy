
#include <ADC.h>

ADC *adc = new ADC();                 // adc object
const int  sensor_gas = A9;         // ADC Methane gas sensor
const int  out_sensor= A7;
const int  sensor_flow = A8;         // flow gas sensor
const int  DAC_ref=A14;             // DAC reference voltage
const byte Num= 50;

float     valueRead=0;
float     valueRead2=0;
float     V_flow=0;;
int       tempADC=0;
int       tempADC2=0;
int       sum_read=0;
float     max_flow =0;
float     min_flow =0;
int       medRead=0;
int       Flag_ADC=0;
int       temp_read[Num]={0};
float     incline;
float     contsB;
float     V_finFlow;
void setup()
{
  delay(3000);
  pinMode(sensor_gas, INPUT);
  pinMode(sensor_flow, INPUT);
  pinMode(out_sensor, INPUT);
  pinMode(DAC_ref, OUTPUT);
  adc->setAveraging(14);                    // set number of averages
  adc->setResolution(14);                   // set bits of resolution
  Serial.begin (38400);
  analogWriteResolution(14);
  CalFlowSensor ();

}
  
void loop()
{
 
  ReadSensorFlow ();
  delay(1000);
  
} // end loop

//******************************************************************************
//                          Calibration of Flow Sensor
//******************************************************************************
void CalFlowSensor ()
{
  analogWrite(DAC_ref,745);
  delay(2000);
  for (int m=0; m<Num; m++)
    {
      temp_read[m] = analogRead(sensor_flow);
      medRead= medRead + temp_read[m];
      delay(100);
    }
    V_flow = medRead;
    V_flow = (V_flow * 3.3)/ 65535;
    min_flow = V_flow/Num;
    max_flow = min_flow + 0.160;
    incline =(max_flow-min_flow)/10;
    contsB = (incline*10)-max_flow;
    Serial.print ("max_flow:");
    Serial.print (max_flow,4);
    Serial.print (" - ");
    Serial.print ("min_flow:");
    Serial.print (min_flow,4);
    Serial.println (" - "); 
    
} // end CalFlowSensor

void ReadSensorFlow ()
{
  tempADC = analogRead(sensor_flow);
  valueRead = tempADC;
  valueRead = (valueRead *3.3)/ 65535;

  tempADC2 = analogRead(out_sensor);
  valueRead2 = tempADC2;
  valueRead2 = (valueRead2 * 3.3)/ 65535;

  V_finFlow = (valueRead + contsB )/incline;
  if (valueRead < min_flow )  V_finFlow= 0;
  if (valueRead > max_flow)  V_finFlow= 10;

  Serial.print ("Vlr read:");
  Serial.print (valueRead2,4);
  Serial.print (" - ");
  Serial.print ("f_sensor (V):");
  Serial.print (valueRead,4);
  Serial.print (" - ");  
  Serial.print ("flow sensor (SLPM):");
  Serial.println (V_finFlow,4);

} // end ReadSensorFlow
