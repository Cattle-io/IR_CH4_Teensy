//======================================================================================
//                               Description
//======================================================================================
// caracteristicas : Comunicación a 38400 Baud, recibe datos del puerto serial,
// revisa si el dato es [AK], en ese caso envia comando Ok, si el dato es diferente
// vuelve a esperar un nuevo comando borrando las variables para que vuelvan a 
// recibir un dato del TERMINAL.
 //======================================================================================
//                              Variables and Constants
//======================================================================================
char miarray[5];
char comp[4]= {'[','A','K',']'};
byte bytesleidos=0;
int flagArray=0;
char rx_byte = 0;
String rx_str = "";
//======================================================================================
//                              Configuration Setup 
//======================================================================================
void setup()
{
          Serial.begin(38400);
          Serial.print("Write Command: ");
}

//======================================================================================
//                               Loop Infinite
//======================================================================================

void loop(){
    if (Serial.available() > 0)                     // is a character available?
    {    
      bytesleidos=Serial.readBytes(miarray,5);
      if (bytesleidos !=4){
          CleanBuff();
          Serial.println("Error");
        }
      else{
        Serial.print("Command:");
        Serial.println(miarray);
        Serial.print("#bytes: ");
        Serial.println(bytesleidos);
        if(CompCommand (comp)){
          Serial.print("ok command");
          }
         else {
          Serial.print("error command");}
      }
     
    }
     CleanBuff();
   
      
    
}// end Loop


//======================================================================================
//                              Functions
//======================================================================================
//******************************************************************************
//                          Clean Buffer 
//******************************************************************************
void CleanBuff (){   // Clean Buffer 5 bytes size
      miarray[0]=0;
      miarray[1]=0;
      miarray[2]=0;
      miarray[3]=0;
      miarray[4]=0;
      bytesleidos=0;
  }
//******************************************************************************
//                    Comparison Command 
//******************************************************************************
int CompCommand (char VlrComp[5]){ 
 int FlVer;                             // Flag=Flver (1:Ok/0:Error)
  if (miarray[0]==VlrComp[0]){
    if (miarray[1]==VlrComp[1]){
      if (miarray[2]==VlrComp[2]){
        if (miarray[3]==VlrComp[3]){
          FlVer=1;
        }
      }
    }
  }
  else FlVer=0;
  return FlVer;
}
//******************************************************************************
//                          Read Command by Buffer
//******************************************************************************
