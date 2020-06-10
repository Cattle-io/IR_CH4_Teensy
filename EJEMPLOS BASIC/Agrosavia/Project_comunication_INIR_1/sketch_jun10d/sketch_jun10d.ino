void setup(){
  Serial.begin(38400); 
}
void loop(){
  if (Serial.available()){
    long num = Serial.readString().toInt(); 
    Serial.println(num);
    Serial.println(sizeof(num));
  }  
}
