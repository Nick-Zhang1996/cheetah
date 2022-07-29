// transducer Serial 1 @921600, 8, 1, no parity <-> Serial 0 @ 115200 
void setup(){
  Serial1.begin(921600, SERIAL_8N1);
  Serial.begin(115200);
}

void loop(){
  while(Serial1.available()){
    Serial.write(Serial1.read());
  }

  while(Serial.available()){
    Serial1.write(Serial.read());
  }
}
