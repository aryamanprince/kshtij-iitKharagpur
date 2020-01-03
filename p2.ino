  #define leftFar 0
  #define leftNear 1
  #define centreFront 2//143   980   591   983   862   859  
  #define rightNear 3//103   225   98   257   147   153   
  #define rightFar 4
  #define centre 5
  
  const int numLeds = 6;
  int speedleft=115;
  int speedright=200;
  int thresholds[numLeds] = {319,475,583,421,363,264};
  int leds[numLeds] = {A0,A1,A2,A3,A4,A5};
  int m11 = 10;  //for left tyre moving backward when high
  int m12 = 11;    //for left tyre moving forward when high
  int m21 = 5;    //for right tyre moving backward when high
  int m22 = 6;   //for right tyre moving forward when high
  int ledStates[numLeds] = {};
  void readLeds(){
    for(int i=0;i < numLeds;i++){
      ledStates[i] = analogRead(leds[i]);
    }
    printLeds();
    putThreshold();
    printLeds();
  }
  void printLeds(){
   Serial.print(ledStates[centreFront]);
   Serial.print("   ");
   Serial.print(ledStates[rightFar]);
   Serial.print("   ");
   Serial.print(ledStates[rightNear]);
   Serial.print("   ");
   Serial.print(ledStates[centre]);
   Serial.print("   ");
   Serial.print(ledStates[leftNear]);
   Serial.print("   ");
   Serial.print(ledStates[leftFar]);
   Serial.print("   ");
   delay(200);
   Serial.println();
  }
  
  
  void Left(){
   
     while(!ledStates[centre])
     {   
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright);
      readLeds();
     }

    
  }

  void Right(){
      while(!ledStates[centre])
      {analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21, 0);
      analogWrite(m22,0);
      readLeds();
      }
    
  }
   
    
  void uTurn() {
    analogWrite(m11,0);
    analogWrite(m12, speedleft);
    analogWrite(m21, 0);
    analogWrite(m22, speedright);
    delay(2);
    
    while(ledStates[leftNear] || ledStates[centre])
    {  analogWrite(m11,speedleft);
    analogWrite(m12,0);
    analogWrite(m21, 0);
    analogWrite(m22, speedright);
    
    readLeds();
    }
  }
  void goStraight(){
    
    analogWrite(m11,0);
    analogWrite(m12,speedleft);
    analogWrite(m21,0);
    analogWrite(m22,speedright);
    
    
  }
  
  void putThreshold(){
    // Thresholding to set the proper values of led states
    for(int i=0;i < numLeds;i++){
      if(ledStates[i] > thresholds[i]){
        ledStates[i] = 1;
      }
      
      else {
        ledStates[i] = 0;
      }
    }
  }
  void stopall(){
    analogWrite(m11,0);
    analogWrite(m12, 0);
    analogWrite(m21, 0);
    analogWrite(m22, 0);
    
    }
    void Back()
    { 
      analogWrite(m11,speedleft-50);
      analogWrite(m12, 0);
      analogWrite(m21, speedright-50);
      analogWrite(m22, 0);
      if(ledStates[rightFar])
      Right();
      else if(ledStates[leftFar])
      Left();
    }
 
  void setup()
  {
Serial.begin(9600);
    
    pinMode(m11,OUTPUT);
    pinMode(m12, OUTPUT);
    pinMode(m21, OUTPUT);
    pinMode(m22, OUTPUT);
  
    for(int i=0;i < numLeds;i++){
      pinMode(leds[i],INPUT);
    }
   
   
  }
  
  
  void loop() {
    
    readLeds();
  
  if((ledStates[centre]&&!ledStates[leftFar]&&!ledStates[leftNear]&&!ledStates[rightNear]&&!ledStates[rightFar]) || 
  (ledStates[centre]&&!ledStates[leftFar]&&(ledStates[leftNear]||ledStates[rightNear])&&!ledStates[rightFar])||
  (!ledStates[centre]&&!ledStates[leftFar]&&(ledStates[leftNear]||ledStates[rightNear])&&!ledStates[rightFar]))
      goStraight();

  if((!ledStates[centre]&&ledStates[leftFar]&&ledStates[leftNear]&&!ledStates[rightNear]&&!ledStates[rightFar]) ||
   (!ledStates[centre]&&ledStates[leftFar]&&!ledStates[leftNear]&&!ledStates[rightNear]&&!ledStates[rightFar])||
   (ledStates[centre]&&ledStates[leftFar]&&ledStates[leftNear]&&!ledStates[rightNear]&&!ledStates[rightFar])||
    (ledStates[leftFar] && ledStates[leftNear]&&ledStates[rightNear]&&!ledStates[rightFar]&& ledStates[centre]))
   Left();
  if((!ledStates[centre]&&!ledStates[leftFar]&&!ledStates[leftNear]&&ledStates[rightNear]&&ledStates[rightFar]) ||
   (!ledStates[centre]&&!ledStates[leftFar]&&!ledStates[leftNear]&&!ledStates[rightNear]&&ledStates[rightFar]) ||
   (ledStates[centre]&&!ledStates[leftFar]&&!ledStates[leftNear]&&ledStates[rightNear]&&ledStates[rightFar])||
    (!ledStates[leftFar] && ledStates[leftNear]&&ledStates[rightNear]&&ledStates[rightFar]&& ledStates[centre]))
   Right();
  
  if(!ledStates[centre]&&!ledStates[leftFar]&&!ledStates[leftNear]&&!ledStates[rightNear]&&!ledStates[rightFar])
   Back();
  } 
    
