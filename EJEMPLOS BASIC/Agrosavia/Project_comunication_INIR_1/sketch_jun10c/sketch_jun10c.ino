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


const long ComA= 0x005B415D;        // Command [A]
const long ComB= 0x005B425D;        // Command [B]
const long ComC= 0x005B435D;        // Command [C]
const long ComI= 0x005B495D;        // Command [I]
const long ComCO= 0x0000005B;        // Command [
const long ComCC= 0x0000005D;        // Command ]
const long ComJ= 0x00000004A;        // Command J
const long ComAK= 0x5B414B5D;        // Command [AK]

const int     NUM_LONGS=5;
const int     NUM_BYTES = 20;
long          byteRecBuff[NUM_LONGS];
long          CommData;
byte          data[NUM_BYTES];
byte          bytesleidos=0;
int           LedOk =12;
int           LedError =11;
int           flagArray=0;


//======================================================================================
//                              Configuration Setup 
//======================================================================================
void setup()
{
  
  pinMode (LedOk,OUTPUT);
  pinMode (LedError,OUTPUT);
  Serial.begin(38400);
  Serial.print("Wait Command: ");
  digitalWrite (LedOk,HIGH); 
  digitalWrite (LedError,HIGH);
  delay(5000);
}

//======================================================================================
//                               Loop Infinite
//======================================================================================

void loop()
{
  digitalWrite (LedOk,LOW); 
  digitalWrite (LedError,LOW);
  CleanBuffdata ();
  CommData=0;
  SendLongData (ComA);                   // Send command [A]
  ReadCommand();
  delay(100);
  if(CommInd(ComAK))
   {
     SendLongData (ComI);
     delay(10);
     if(TryGetSerialData(data,NUM_BYTES))
      {
       byteToInt(); 
       digitalWrite (LedOk,HIGH); 
       digitalWrite (LedError,LOW);
       Serial.print ("Dato 1:");
       Serial.println(byteRecBuff[0],HEX);
       Serial.print ("Dato 2:");
       Serial.println(byteRecBuff[1],HEX);
       Serial.print ("Dato 3:");
       Serial.println(byteRecBuff[2],HEX);
       Serial.print ("Dato 4:");
       Serial.println(byteRecBuff[3],HEX);
       Serial.print ("Dato 5:");
       Serial.println(byteRecBuff[4],HEX);
       delay(300);
      }
    }
      
    
}// end Loop


//======================================================================================
//                              Functions
//======================================================================================
//******************************************************************************
//                          Send logn data by buffer 
//******************************************************************************
void SendLongData (long DataSend){   // send data by buffer
  int temp;  
  
  temp = DataSend >> 16;               // separate 16 bits high
  Send2Bytes(temp);
  temp= DataSend & 0xFFFF;             // separate 16 bits low
  Send2Bytes(temp);
  
 }// end SendLongData
//******************************************************************************
//                          Send int data by buffer 
//*****************************************************************************
 void Send2Bytes(uint16_t value)
{
  Serial.write(highByte(value));
  Serial.write(lowByte(value));  
} 
//******************************************************************************
//                          Clean Buffer 
//******************************************************************************
void CleanBuffdata (){   // Clean Buffer 
      for (int i=0; i<NUM_BYTES; i++)
      {
        data[i]=0;      
      }
  }// end CleanBuffdata
//******************************************************************************
//                    Comparison Command in Buffer
//******************************************************************************
// (data to comp / position buffer )
int CompCommand (long databuff,int posbuff){ 
 int FlVer =0;                                    // Flag=Flver (1:Ok/0:Error)
 if (byteRecBuff[posbuff] == databuff)
  {
    FlVer = 1;   // It´s equal
  }
 else 
  {
    FlVer = 0;   // It´s different 
  }
 return FlVer;

}// end CompCommand

//******************************************************************************
//                    Comparison Command individual
//******************************************************************************
bool CommInd (long databuff)
{
  if (CommData == databuff)
  {
    return true;
  }
  CommData=0;
  return false;
  
}
//******************************************************************************
//                          Read Command 
//******************************************************************************
void ReadCommand ()
{
  if(Serial.available() >=4)
  {
    byte byte1 = Serial.read();
    byte byte2 = Serial.read();
    byte byte3 = Serial.read();
    byte byte4 = Serial.read();
    CommData= bytetoLong (byte1, byte2, byte3, byte4);
    
  }  return CommData;
}// end ReadCommand

//******************************************************************************
//                          Read Command by Buffer
//******************************************************************************

bool TryGetSerialData( byte* data, uint8_t dataLength)
{
   if (Serial.available() >= dataLength)
   {
      Serial.readBytes(data, dataLength);
      return true;
   }
   return false;
} // end TryGetSerialData
//******************************************************************************
//                         Convertion byte to long
//******************************************************************************
long bytetoLong (byte Byte1,byte Byte2,byte Byte3,byte Byte4)
{
  return (long)Byte1 << 24 | (long)Byte2 << 16 |(long)Byte3 << 8 | (long)Byte4;
}// end bytetoLong
//******************************************************************************
//                         Convertion buffer to long
//******************************************************************************
long byteToInt()
{ 
  byteRecBuff[0]= (long)data[0] << 24 | (long)data[1] << 16 |(long)data[2]<< 8 | (long)data[3];
  byteRecBuff[1]= (long)data[4] << 24 | (long)data[5] << 16 |(long)data[6]<< 8 | (long)data[7];
  byteRecBuff[2]= (long)data[8] << 24 | (long)data[9] << 16 |(long)data[10]<< 8 | (long)data[11];
  byteRecBuff[3]= (long)data[12] << 24 | (long)data[13] << 16 |(long)data[14]<< 8 | (long)data[15];
  byteRecBuff[4]= (long)data[16] << 24 | (long)data[17] << 16 |(long)data[18]<< 8 | (long)data[19];
  return byteRecBuff;
}// end byteToInt
