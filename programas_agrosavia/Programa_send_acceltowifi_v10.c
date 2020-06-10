////====================================================================================
////-- Libraries Included --------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <WiFiUdp.h>

////------------------------------------------------------------------------------------
//// Authentication Variables
const char* ssid = "ProyectoAgrosavia1";
const char* password = "agrosavia";

#define     SerialPort  9001
WiFiUDP     Udp;

////------------------------------------------------------------------------------------
#define     LED0      2           // WIFI Module LED
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;
int contconexion = 0;

// Select SDA and SCL pins for I2C communication
const uint8_t scl = D6;
const uint8_t sda = D7;

// sensitivity scale factor respective to full scale setting provided in datasheet
const uint16_t AccelScaleFactor = 2048;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;


void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}


//configure MPU6050
void MPU6050_Init() {
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
   delay(50);
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Wire.begin(sda, scl);
  MPU6050_Init();
  // Setting The Mode Of Pins
  pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light
          // Indicator For Client #1 Connectivity

 

  pinMode(5, OUTPUT);  //D1 Led de estado
  digitalWrite(15, LOW);

  WiFi.mode(WIFI_STA); //para que no inicie el SoftAP en el modo normal
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED and contconexion < 1000) { //Cuenta hasta 50 si no se puede conectar lo cancela
    ++contconexion;
    delay(250);
    Serial.print(".");
    digitalWrite(LED0, HIGH);
    delay(250);
    digitalWrite(LED0, LOW);
  }
  if (contconexion < 1000) {
    //para usar con ip fija
    IPAddress Ip(192,168,0,150); //ip node
    IPAddress Gateway(192,168,0,1); //ip del router
    IPAddress Subnet(255,255,255,0);
    WiFi.config(Ip, Gateway, Subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
    digitalWrite(LED0, HIGH);
  }
  else {
    Serial.println("");
    Serial.println("Error de conexion");
    digitalWrite(15, LOW);
  }
}


void loop() {
  EnvioDeDatos();
}


// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
  AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
  Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
  GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());
}



void EnvioDeDatos()
{
  double Ax, Ay, Az, T, Gx, Gy, Gz;
  String msg = "";
  while (true)
  {
    Udp.beginPacket("192.168.0.102", 9001); //ip del pc
    //JClient.connect(JServer, 9001);
    Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);

    //divide each with their sensitivity scale factor
    Ax = (double)AccelX / AccelScaleFactor;
    Ay = (double)AccelY / AccelScaleFactor;
    Az = (double)AccelZ / AccelScaleFactor;

    //Ax = (double)16;
    //Ay = (double)50;
    //Az = (double)200;

    Serial.print("Ax: "); Serial.print(Ax);
    Serial.print(" Ay: "); Serial.print(Ay);
    Serial.print(" Az: "); Serial.println(Az);
    msg = String(Ax) + "#" + String(Ay) + "#" + String(Az);
    for (int i = 0; i < msg.length(); i++)
    {
      int old = micros();
      Udp.write(msg[i]);
      while (micros() - old < 87);
    }
    Udp.endPacket();
    //JClient.print(msg);
    delay(50);
    //JClient.stop();
  }

}


//====================================================================================


//====================================================================================
