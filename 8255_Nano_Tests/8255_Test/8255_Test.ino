void setup() {
  Serial.begin(9600);
  for(int i=2; i<=9; i++){
    pinMode(i, OUTPUT);
  }  
  pinMode(12,OUTPUT);
}

void loop() {
//  sendIOCommand();
//  digitalWrite(12,0);
//  Serial.println("IO command on bus");
  delay(1);

//  sendBSRCommand();
  sendData();
  
 while(1){
  
 }
}

void sendIOCommand(){
  digitalWrite(2,0);
  digitalWrite(3,0);
  digitalWrite(4,0);
  digitalWrite(5,0);
  digitalWrite(6,0);
  digitalWrite(7,0);
  digitalWrite(8,0);
  digitalWrite(9,1);
}

void sendBSRCommand(){
  digitalWrite(2,1);
  digitalWrite(3,1);
  digitalWrite(4,1);
  digitalWrite(5,0);
  digitalWrite(6,0);
  digitalWrite(7,0);
  digitalWrite(8,0);
  digitalWrite(9,0);
}

void sendData(){
  digitalWrite(2,1);
  digitalWrite(3,0);
  digitalWrite(4,1);
  digitalWrite(5,0);
  digitalWrite(6,1);
  digitalWrite(7,0);
  digitalWrite(8,1);
  digitalWrite(9,0);
}
