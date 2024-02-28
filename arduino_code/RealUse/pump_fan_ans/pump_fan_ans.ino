//10 channel

long randNumber; // Choose Channel randomly
#define VOC_DURATION 10 //10sec
#define WASH_DURATION 600 //010min

void setup() {
  pinMode(2,OUTPUT); //PUMP
  pinMode(3,OUTPUT); //FAN


  //randomSeed (analogRead(10));
  // String status;
  Serial.begin(115200);
  digitalWrite(2, LOW); //pump멈춤
  digitalWrite(3, LOW); //fan멈춤

  String status;
}

void loop(){
  digitalWrite(2, LOW); //pump멈춤
  digitalWrite(3, LOW); //fan멈춤
  
  for (int k=2; k <3; k++){
    randNumber = k;
    valve(randNumber);
    delay(1000); 
  }
}

void valve(int val){
  String status;

  if (val==2){  
    digitalWrite(2, HIGH); //pump가동
    digitalWrite(3, LOW); //팬멈춤
    status = "{\"VOC\":"+String('1')+"}";
    // Serial.println(val);
    Serial.println(status);
    for (int i = 1; i < VOC_DURATION; i++) {
      delay(1000);
      }
    digitalWrite(2, LOW); //pump멈춤
    digitalWrite(3, HIGH); //fan가동
    status = "{\"VOC\":"+String('2')+"}";
    // Serial.println(val);
    Serial.println(status);
    for (int i = 1; i < WASH_DURATION; i++) {
      delay(1000);
      }

    //digitalWrite(2, LOW); //pump멈춤
    //digitalWrite(3, HIGH); //fan가동
    //status = "{\"VOC\":"+String('3')+"}";
    // Serial.println(val);
    //Serial.println(status);
    //for (int i = 1; i < WASH_DURATION; i++) {
      //delay(1000);
      //}
  } 

  else{
   Serial.println("else");
  }
}
  