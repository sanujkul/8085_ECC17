
//LCD without Library:
//Pin connections using MEGA
//22 - DB1
//23 - DB0 
//D2-D7 -  DB2 - DB7
//D9 - E;
//D10- RS
//D11- R/~W

void setup() {
  Serial.begin(9600);
  for(int i=0; i<=11; i++){
    pinMode(i,OUTPUT);
  }
  digitalWrite(9, HIGH);
  
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
}

void loop() {
  delay(3000); //Delay for 10 sec to conncect wire
  Serial.println("start");
//  String data = "0000111000";
//  Serial.println(data);
               //SW76543210
  sendDataroLCD("0000111000");
  sendDataroLCD("0000001110");
  sendDataroLCD("0000000110");
  
  sendDataroLCD("1001001000"); //H
  sendDataroLCD("1001000101"); //E
  sendDataroLCD("1001001100"); //L
  sendDataroLCD("1001001100"); //L
  sendDataroLCD("1001001111"); //0

  sendDataroLCD("1000100000");
  
  sendDataroLCD("1001010011"); //S
  sendDataroLCD("1001110101"); //U
  sendDataroLCD("1001101101"); //M
  sendDataroLCD("1001101001"); //I
  
  sendDataroLCD("1000100001"); //!
  delay(5000);

  sendDataroLCD("0000000111");
  sendDataroLCD("1001001000"); //H
  sendDataroLCD("1001000101"); //E
  sendDataroLCD("1001001100"); //L
  sendDataroLCD("1001001100"); //L
  sendDataroLCD("1001001111"); //0

  sendDataroLCD("1000100000");
  
  sendDataroLCD("1001010011"); //S
  sendDataroLCD("1001110101"); //U
  sendDataroLCD("1001101101"); //M
  sendDataroLCD("1001101001"); //I
  
  sendDataroLCD("1000100001"); //!

  delay(10000);
  
  //clearing
  Serial.println("clearing");
  sendDataroLCD("0000000001");
  
  
  
  while(1){
    
  }
}


void sendDataroLCD(String data){
  Serial.println(data);
  data = invertString(data);
  Serial.println(data);
  //Now left->Least Significant, and Right->Most Significant
  
  //int c = String(data.charAt(0)).toInt();
  //Serial.println(c);
  
  for(int i=0; i<=7; i++){
    int d = String(data.charAt(i)).toInt();
    int pin = i;
    if(i==0){
      pin = 23;
    }else if(pin == 1){
      pin = 22;
    }
    Serial.println("(i,d) = "+String(pin)+","+String(d));
    digitalWrite(pin,d);
  }

  int RW = String(data.charAt(8)).toInt();
  digitalWrite(11,RW);
  
  int RS = String(data.charAt(9)).toInt();
  digitalWrite(10,RS);

  enableLCD();
  
  waitUntilBusy();

  delay(100);
}

String invertString(String data){
  for(int i=0; i<=4; i++){
    char c = data.charAt(i);
    data.setCharAt(i,data.charAt(9-i));
    data.setCharAt(9-i,c);
  }

  return data;
}

void waitUntilBusy(){
  String LCDReadData = "00000000";
  Serial.println("========>>"+String(digitalRead(7)));
  while(digitalRead(7) == HIGH){
    //wait
    Serial.println("busy");
  }

  //wait over. Return back
}

void enableLCD(){
  digitalWrite(9, LOW);
  delay(5);
  digitalWrite(9,HIGH);
}
