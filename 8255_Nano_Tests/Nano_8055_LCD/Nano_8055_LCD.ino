
#define CS 10
#define A1 11
#define A0 12
#define WR 13

void setup() {
  for(int i=2; i<=13; i++){
    pinMode(i, OUTPUT);
  }
  digitalWrite(WR, HIGH);
}

void loop() {
// I. Setting up 8055 
  //Send 80H to CNTRL Reg of 8055: All are in output mode
  sendDataToCntrlReg("10000000"); //IO Mode
  delay(4);
  //PC0 -> HIGH, //LCD enable -> HIGH 
  sendDataToCntrlReg("00000001"); //BSR Mode

  //Testing
  //sendDataToPortB("10101000"); //Working!
// II. Setting up LCD:
  //Sending Function set
  writeToLCDCommandReg("00111000"); //38H sending 
  delay(1);
  //Display On/Off
  writeToLCDCommandReg("00001110"); //0EH sending
  delay(1);
  //Sending Entry Mode Set
  writeToLCDCommandReg("00000111"); //06H sending
  delay(1);
  //Sending Function set
  writeToLCDCommandReg("00000001"); //06H sending
  delay(1);  
  //Start writing from 8th position
  writeToLCDCommandReg("10001111");
// III. Writing Data to Data Reg
  writeToLCDDataReg("01010100");
  writeToLCDDataReg("01001000");
  writeToLCDDataReg("01001001");
  writeToLCDDataReg("01010011");
  
  writeToLCDDataReg("00100000");

  writeToLCDDataReg("01001001");
  writeToLCDDataReg("01010011");

  writeToLCDDataReg("00100000");

  writeToLCDDataReg("01000111");
  writeToLCDDataReg("01010000");
  writeToLCDDataReg("01010011");

  writeToLCDDataReg("00100000");
  
//Entry Mode set: I/D = 1-> Increment cursor, S=1 -> shift display
  //writeToLCDCommandReg("00000111");
  
  
  writeToLCDDataReg("01000111");
  writeToLCDDataReg("01010101");
  writeToLCDDataReg("01001001");
  writeToLCDDataReg("01000100");
  writeToLCDDataReg("01000101");
  writeToLCDDataReg("01010010");
  
  //againWritingSamething();
  
  while(1){
    
  }
}

void againWritingSamething(){

  //Sending Entry Mode Set
  writeToLCDCommandReg("00000111"); //06H sending
  
  writeToLCDDataReg("01010100");
  writeToLCDDataReg("01001000");
  writeToLCDDataReg("01001001");
  writeToLCDDataReg("01010011");
  
  writeToLCDDataReg("00100000");

  writeToLCDDataReg("01001001");
  writeToLCDDataReg("01010011");

  writeToLCDDataReg("00100000");

  writeToLCDDataReg("01000111");
  writeToLCDDataReg("01010000");
  writeToLCDDataReg("01010011");

  writeToLCDDataReg("00100000");
}
//////////////////////// LCD ////////////////////////////////////////
void writeToLCDDataReg(String Data){
  sendDataToPortB(Data);

  //Setting RS (PC1) LOW
  sendDataToCntrlReg("00000011"); //BSR Mode
  
  lcdCntrlSignals();

  delay(500);
}


void writeToLCDCommandReg(String Data){
  sendDataToPortB(Data);
  
  //Setting RS (PC1) LOW
  sendDataToCntrlReg("00000010"); //BSR Mode
  
  lcdCntrlSignals();
}

void lcdCntrlSignals(){
  //Setting WR (PC2) LOW
  sendDataToCntrlReg("00000100"); //BSR Mode

  delay(2);

  //Data is written by falling edge on EN pin
  sendDataToCntrlReg("00000000"); //BSR Mode
  delay(2);
  sendDataToCntrlReg("00000001"); //BSR Mode
}

//////////////////////////////////////////8255///////////////////////////////

void sendDataToPortB(String data){
  putDataOnDataBus(data);
  setPortBAddress(); 
  csAndWrite();
}

void sendDataToCntrlReg(String data){
  putDataOnDataBus(data);
  setCNTRLRegAddress(); 
  csAndWrite();
}

void csAndWrite(){
  digitalWrite(CS, LOW);
  delay(1);
  digitalWrite(WR, LOW);
  delay(4);
  digitalWrite(WR, HIGH);
  digitalWrite(CS, HIGH);
}

void setCNTRLRegAddress(){
  digitalWrite(A1,HIGH);
  digitalWrite(A0,HIGH);
}

void setPortBAddress(){
  digitalWrite(A1,LOW);
  digitalWrite(A0,HIGH);
}

void putDataOnDataBus(String data){
 int Dcount = 9;
 int dataIndex = 0; //at 0 index of string there is most significant bit

 for(Dcount=9; Dcount>=2; Dcount--){
  int d = String(data.charAt(dataIndex)).toInt();
  digitalWrite(Dcount, d);
  dataIndex++;
 }
}

void enableLCD(){
  //digitalWrite(9, LOW);
  delay(5);
  //digitalWrite(9,HIGH);
}
