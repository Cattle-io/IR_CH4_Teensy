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

const int     NUM_LONGS=38;
const int     NUM_BYTES = 152;
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
  
  digitalWrite (LedOk,HIGH); 
  digitalWrite (LedError,LOW);
  delay(3000);
  CleanBuffdata ();                       // clean complete Buffer
  CommData=0;                             // clean variable command
  SendLongData (ComA);                   // Send command [A]
  digitalWrite (LedOk,LOW); 
  digitalWrite (LedError,HIGH);
  delay(2000);
}

//======================================================================================
//                               Loop Infinite
//======================================================================================

void loop()
{
  digitalWrite (LedOk,HIGH); 
  digitalWrite (LedError,HIGH);
  ReadCommand();                         // Read Command received
  delay(100);
  if(CommInd(ComAK))                      // compare command [AK]
   {
     digitalWrite (LedOk,HIGH); 
     digitalWrite (LedError,LOW);
     SendLongData (ComI);                 // Send command [I]
     delay(1000);
     if(TryGetSerialData(data,NUM_BYTES))  // Read Answer Configuration Mode 
      {
       delay(200);
       byteToInt(); 
       digitalWrite (LedOk,LOW); 
       digitalWrite (LedError,HIGH);
       digitalWrite (LedOk,HIGH); 
       digitalWrite (LedError,LOW);
       Serial.print ("Dato 1:");
       Serial.println(byteRecBuff[0],HEX);
       Serial.print ("Dato 2:");
       Serial.println(byteRecBuff[1],HEX);
       Serial.print ("Dato 21:");
       Serial.println(byteRecBuff[20],HEX);
       Serial.print ("Dato 31:");
       Serial.println(byteRecBuff[30],HEX);
       Serial.print ("Dato 38:");
       Serial.println(byteRecBuff[37],HEX);
       delay(1000);
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
  byteRecBuff[5]= (long)data[20] << 24 | (long)data[21] << 16 |(long)data[22]<< 8 | (long)data[23];
  byteRecBuff[6]= (long)data[24] << 24 | (long)data[25] << 16 |(long)data[26]<< 8 | (long)data[27];
  byteRecBuff[7]= (long)data[28] << 24 | (long)data[29] << 16 |(long)data[30]<< 8 | (long)data[31];
  byteRecBuff[8]= (long)data[32] << 24 | (long)data[33] << 16 |(long)data[34]<< 8 | (long)data[35];
  byteRecBuff[9]= (long)data[36] << 24 | (long)data[37] << 16 |(long)data[38]<< 8 | (long)data[39];
  byteRecBuff[10]= (long)data[40] << 24 | (long)data[41] << 16 |(long)data[42]<< 8 | (long)data[43];
  byteRecBuff[11]= (long)data[44] << 24 | (long)data[45] << 16 |(long)data[46]<< 8 | (long)data[47];
  byteRecBuff[12]= (long)data[48] << 24 | (long)data[49] << 16 |(long)data[50]<< 8 | (long)data[51];
  byteRecBuff[13]= (long)data[52] << 24 | (long)data[53] << 16 |(long)data[54]<< 8 | (long)data[55];
  byteRecBuff[14]= (long)data[56] << 24 | (long)data[57] << 16 |(long)data[58]<< 8 | (long)data[59];
  byteRecBuff[15]= (long)data[60] << 24 | (long)data[61] << 16 |(long)data[62]<< 8 | (long)data[63];
  byteRecBuff[16]= (long)data[64] << 24 | (long)data[65] << 16 |(long)data[66]<< 8 | (long)data[67];
  byteRecBuff[17]= (long)data[68] << 24 | (long)data[69] << 16 |(long)data[70]<< 8 | (long)data[71];
  byteRecBuff[18]= (long)data[72] << 24 | (long)data[73] << 16 |(long)data[74]<< 8 | (long)data[75];
  byteRecBuff[19]= (long)data[76] << 24 | (long)data[77] << 16 |(long)data[78]<< 8 | (long)data[79];
  byteRecBuff[20]= (long)data[80] << 24 | (long)data[81] << 16 |(long)data[82]<< 8 | (long)data[83];
  byteRecBuff[21]= (long)data[84] << 24 | (long)data[85] << 16 |(long)data[86]<< 8 | (long)data[87];
  byteRecBuff[22]= (long)data[88] << 24 | (long)data[89] << 16 |(long)data[90]<< 8 | (long)data[91];
  byteRecBuff[23]= (long)data[92] << 24 | (long)data[93] << 16 |(long)data[94]<< 8 | (long)data[95];
  byteRecBuff[24]= (long)data[96] << 24 | (long)data[97] << 16 |(long)data[98]<< 8 | (long)data[99];
  byteRecBuff[25]= (long)data[100] << 24 | (long)data[101] << 16 |(long)data[102]<< 8 | (long)data[103];
  byteRecBuff[26]= (long)data[104] << 24 | (long)data[105] << 16 |(long)data[106]<< 8 | (long)data[107];
  byteRecBuff[27]= (long)data[108] << 24 | (long)data[109] << 16 |(long)data[110]<< 8 | (long)data[111];
  byteRecBuff[28]= (long)data[112] << 24 | (long)data[113] << 16 |(long)data[114]<< 8 | (long)data[115];
  byteRecBuff[29]= (long)data[116] << 24 | (long)data[117] << 16 |(long)data[118]<< 8 | (long)data[119];
  byteRecBuff[30]= (long)data[120] << 24 | (long)data[121] << 16 |(long)data[122]<< 8 | (long)data[123];
  byteRecBuff[31]= (long)data[124] << 24 | (long)data[125] << 16 |(long)data[126]<< 8 | (long)data[127];
  byteRecBuff[32]= (long)data[128] << 24 | (long)data[129] << 16 |(long)data[130]<< 8 | (long)data[131];
  byteRecBuff[33]= (long)data[132] << 24 | (long)data[133] << 16 |(long)data[134]<< 8 | (long)data[135];
  byteRecBuff[34]= (long)data[136] << 24 | (long)data[137] << 16 |(long)data[138]<< 8 | (long)data[139];
  byteRecBuff[35]= (long)data[140] << 24 | (long)data[141] << 16 |(long)data[142]<< 8 | (long)data[143];
  byteRecBuff[36]= (long)data[144] << 24 | (long)data[145] << 16 |(long)data[146]<< 8 | (long)data[147];
  byteRecBuff[37]= (long)data[148] << 24 | (long)data[149] << 16 |(long)data[150]<< 8 | (long)data[151];
 
  return byteRecBuff;
}// end byteToInt
