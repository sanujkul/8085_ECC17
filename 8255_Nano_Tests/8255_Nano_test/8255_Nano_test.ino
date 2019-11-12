
#define IOW 8
#define IOR 9

#define CS 10
#define add1 11
#define add0 12
//Port A : 10->0, add0->0, add1->0
//Port B : 10->0, add0->0, add1->1
//Port C : 10->0, add0->1, add1->0
//Control Reg : 10->0, add0->1, add1->1

String A_Reg = "00000000";
String B_Reg = "00000000";

void setup() {
  Serial.begin(9600);
  for(int i=0; i<=12; i++){
    pinMode(i,OUTPUT);
  }
  digitalWrite(CS,HIGH);
  digitalWrite(IOW,HIGH);
  digitalWrite(13,HIGH);
}

void loop() {
  bookCode();

  while(1);
}

//============================= //============================= //============================= //============================= 
void bookCode(){
  writeToCtrl("10010000"); //Setting portA as Input, portB as Output and C as output
  writeToCtrl("00000000"); //Setting EN of LCD LOW (making PC0 -> LOW)
  INIT();
}

void INIT(){
  A_Reg = "00110000";
  B_Reg = A_Reg;
  OUT_PUT();           //DB7 cant be checked before above instruction
  CMDOUT();            //Send to port B two more times
  CMDOUT();

  A_Reg = "00111000"; //Function set for 8 bit and 2 lines
  CMDOUT();

  A_Reg = "00001000"; //code for display off
  CMDOUT();

  A_Reg = "00000001"; //code for clear display
  CMDOUT();

  A_Reg = "00000110"; //Entry mode set; shift and increment cursor
  CMDOUT();

  A_Reg = "00001110"; //code to turn on display, cursor and blink
  CMDOUT();

  DSPLAY();
}

void DSPLAY(){
  A_Reg = "10000111"; //Write to 8th locaton of DDRAM
  CMDOUT();

  NEXT();
}

void NEXT(){
  A_Reg = "01001000";
  DTAOUT();

  A_Reg = "01000101";
  DTAOUT();

  A_Reg = "01001100";
  DTAOUT();

  A_Reg = "01001100";
  DTAOUT();

  A_Reg = "01001111";
  DTAOUT();
}

void CMDOUT(){
  B_Reg = A_Reg;
  CHKDB7();
  OUT_PUT();
}

void OUT_PUT(){
  A_Reg = "00000010"; //Select command register PC1=0 (that makes RS = 0)
  writeToCtrl(A_Reg);

  A_Reg = "00000100"; //Select command register PC2=0 (that makes R/W = 0)
  writeToCtrl(A_Reg);

  A_Reg = "00000001"; //Select command register PC0=1 (that makes EN = 1)
  writeToCtrl(A_Reg);

  A_Reg = B_Reg;
  writeToPortB(A_Reg);

  A_Reg = "00000000"; //Select command register PC0=0 (that makes EN = 0)
  writeToCtrl(A_Reg);
}

void DTAOUT(){
  B_Reg = A_Reg;
  CHKDB7();
  
  A_Reg = "00000011"; //Select data register PC1=1 (that makes RS = 1)
  writeToCtrl(A_Reg);

  A_Reg = "00000100"; //Select command register PC2=0 (that makes R/W = 0)
  writeToCtrl(A_Reg);

  A_Reg = "00000001"; //Select command register PC0=1 (that makes EN = 1)
  writeToCtrl(A_Reg);

  A_Reg = B_Reg;
  writeToPortB(A_Reg);

  A_Reg = "00000000"; //Select command register PC0=0 (that makes EN = 0)
  writeToCtrl(A_Reg);
}

void CHKDB7(){
  A_Reg = "10010010"; //Set up port B as input port
  writeToCtrl(A_Reg);

  A_Reg = "00000010"; //Select command register PC1=0 (that makes RS = 0)
  writeToCtrl(A_Reg);

  A_Reg = "00000101"; //Enable Read PC2=1 (that makes R/~W = 1)
  writeToCtrl(A_Reg);

  READ();
}

void READ(){
  A_Reg = "00000001"; //Set EN high
  writeToCtrl(A_Reg);

  int flag = digitalRead(7);

  A_Reg = "00000000"; //Set EN LOW
  writeToCtrl(A_Reg);  

  if(flag == 1){
    READ();
    digitalWrite(13,LOW);
  }

  A_Reg = "10000000"; //Set up port B as output port
  writeToCtrl(A_Reg);
  
  digitalWrite(13,HIGH);
}

//============================= //============================= //============================= //============================= 
void writeToCtrl(String data){
//  Serial.println(data);
  data = invertString(data);
//  Serial.println(data);
  //Now left(index = 0)->Least Significant, and Right(index = 1)->Most Significant
  
  //int c = String(data.charAt(0)).toInt();
  //Serial.println(c);
  
//============================= Putting data on data lines  
  putDataOnDatabus(data);
//=============================
  digitalWrite(CS,LOW);
  digitalWrite(add0,HIGH);
  digitalWrite(add1,HIGH);
  digitalWrite(IOW,LOW);
  delay(10);
  digitalWrite(IOW,HIGH);
}

void writeToPortB(String data){
//  Serial.println(data);
  data = invertString(data);
//  Serial.println(data);
  //Now left(index = 0)->Least Significant, and Right(index = 1)->Most Significant
  
  //int c = String(data.charAt(0)).toInt();
  //Serial.println(c);
  
//============================= Putting data on data lines  
  putDataOnDatabus(data);
//=============================
  digitalWrite(CS,LOW);
  digitalWrite(add0,LOW);
  digitalWrite(add1,HIGH);
  digitalWrite(IOW,LOW);
  delay(10);
  digitalWrite(IOW,HIGH);
}

void putDataOnDatabus(String data){
  for(int i=0; i<=7; i++){
    int d = String(data.charAt(i)).toInt();
//    Serial.println("(i,d) = "+String(pin)+","+String(d));
    digitalWrite(i,d);
  }
}

String invertString(String data){
  for(int i=0; i<=3; i++){
    char c = data.charAt(i);
    Serial.println(c);
    data.setCharAt(i,data.charAt(7-i));
    data.setCharAt(7-i,c);
  }

  return data;
}
