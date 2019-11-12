#define IOW 8
#define IOR 9

#define CS 10
#define add1 11
#define add0 12
//Port A : 10->0, add0->0, add1->0
//Port B : 10->0, add0->0, add1->1
//Port C : 10->0, add0->1, add1->0
//Control Reg : 10->0, add0->1, add1->1

void setup(){
  Serial.begin(9600);
  for(int i=0; i<=13; i++){
    pinMode(i,OUTPUT);
  }

  digitalWrite(CS, HIGH);
  digitalWrite(IOW,HIGH);
  digitalWrite(IOR,HIGH);
  digitalWrite(13, HIGH);
  
}

void loop(){
  writeToCtrl("10010000"); //Setting portA as Input, portB as Output and C as output

  BSR_Test();

  writeToPortB("11111111");
  
  while(1);
}

void BSR_Test(){
  //BSR TEST
  writeToCtrl("00001111"); //Setting Pc7 = 1
  writeToCtrl("00001100"); //Setting Pc6 = 0
  writeToCtrl("00001011"); //Setting Pc5 = 1
  writeToCtrl("00001000"); //Setting Pc4 = 0
  writeToCtrl("00000111"); //Setting Pc3 = 1
  writeToCtrl("00000100"); //Setting Pc2 = 0
  writeToCtrl("00000011"); //Setting Pc1 = 1
  delay(5000);
  writeToCtrl("00000000"); //Setting Pc0 = 0
  
  writeToCtrl("00001111"); //Setting Pc7 = 1
}

void writeToCtrl(String data){
  Serial.println(data);
  data = invertString(data);
  Serial.println(data);
  //Now left(index = 0)->Least Significant, and Right(index = 1)->Most Significant
  
  //int c = String(data.charAt(0)).toInt();
  //Serial.println(c);
  
//============================= Putting data on data lines  
  putDataOnDatabus(data);
//=============================
  digitalWrite(add0,HIGH);
  digitalWrite(add1,HIGH);
  
  digitalWrite(CS,LOW);
  digitalWrite(IOW,LOW);
  delay(100);q
  digitalWrite(CS,HIGH);
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
  
  digitalWrite(add0,LOW);
  digitalWrite(add1,HIGH);
  
  digitalWrite(CS,LOW);
  digitalWrite(IOW,LOW);
  delay(100);
  digitalWrite(CS,HIGH);
  digitalWrite(IOW,HIGH);
  
}
void putDataOnDatabus(String data){
  Serial.println("Putting this data on data lines:"+data);
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
