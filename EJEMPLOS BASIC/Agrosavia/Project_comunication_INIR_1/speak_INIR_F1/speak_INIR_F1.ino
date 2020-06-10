// comunicación serial recibe el buffer, y espera un valor para revisar si el dato ingresado anteriormente
// es valido, de lo contrario debera ingresar un nuevo dato y el dato de parada (0x71), para que vuelva a 
// analizar y asi repetidamente.

void setup() {
  Serial.begin(38400);
  Serial.println("Esperando comunicacion...");
}

char rx_byte = 0;
String rx_str = "";
long f =0;
char longtbuf[32];

void loop() {
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();       // get the character
    
    if (rx_byte != 0x71) {
      // a character of the string was received
      rx_str += rx_byte;
      Serial.print("Wait command...");
   }
   else {
      // end of string
     
      if (rx_str == "[AK]")
      {
        Serial.print("Welcome ");
        Serial.println(rx_str);
        rx_str = "";                // clear the string for reuse
        Serial.println("");
        Serial.println("esperando nuevo comando.");
      }
      else
      {
        rx_str = "";                // clear the string for reuse
        Serial.println("");
        Serial.println("error no reconone.");
        Serial.println(f);
      }
    }
  } // end: if (Serial.available() > 0)
}
