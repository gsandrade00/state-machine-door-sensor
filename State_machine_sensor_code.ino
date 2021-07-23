 char switchState = 0;
char lastSwitchState = 1;

void setup() {
 
Serial.begin(9600);
}

void loop() {

 if (Serial.available()) {
  switchState = Serial.read();
 
  if (switchState != lastSwitchState) {  
    if (switchState == '1') {
        delay(250);
        Serial.print("Porta aberta");
        lastSwitchState = switchState;
    } else if (switchState == '0') {
        delay(250);
        Serial.print("Porta fechada");
        lastSwitchState = switchState;
    }
  } else {
     
  }
 }
}

 
