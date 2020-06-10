
//==================Variables and Constants ===============================
long Buffer_Read =0;
long Command_Send =0;
long Data_Recept =0;
long Vlr_comp= 0x5B414B5D;

//================== Setup ===============================
void setup() 
{
  Serial.begin(38400);  // by default 38400 Baud - INIR-ME-100%
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  analogReference(12); // define 12 bits  Analogue Port
  EstablishContact(); 
 
 
}// end setup
//================== Loop infinity ===============================
void loop() 
{
    if (Serial.available() > 8) {
   Buffer_Read = Serial.read();
  }
   if (Buffer_Read==Vlr_comp)  // Response [AK] is OK
  {
    Serial.write("[I]"); // to read back the settings
    delay(1000);
    
   }
  else{  
  EstablishContact();
  Serial.print(Buffer_Read);
  }
  
  delay(1);

}// end loop
//================== Functions ===============================
void EstablishContact() 
{
  Serial.write("[C]"); // put the sensor into Configuration mode
  delay(1000);

}



  

  
  
