  #define leftFar 5
  #define leftNear 4
  #define centreFront 0
  #define rightNear 2
  #define rightFar 1
  #define center 3
  
  const int numLeds = 6;
  int speedleft=150;
  int speedright=170;
  int thresholds[numLeds] = {250,432,251,260,240,283};
  int leds[numLeds] = {A0,A1,A2,A3,A4,A5};
  int m11 = 10;  //for left tyre moving backward when high
  int m12 = 11;    //for left tyre moving forward when high
  int m21 = 6;    //for right tyre moving backward when high
  int m22 = 5;   //for right tyre moving forward when high
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
   Serial.print(ledStates[center]);
   Serial.print("   ");
   Serial.print(ledStates[leftNear]);
   Serial.print("   ");
   Serial.print(ledStates[leftFar]);
   Serial.print("   ");
   delay(100);
   //135 118 132 97 150 154 all black
   //604   644   561   577   864   700 all white
   //134   172   198   180   177   534 straight
   //117   114   174   152   697   151 right far line and slightly right near
   //120   119   332   346   505   164 right near on line
   //447   139   126   88   136   127  left far on line
   //262   445   133   97   152   154  left near on line
    Serial.println();
  }
  
  
  void turnLeft(){
    while(ledStates[center])
    {
     analogWrite(m11,0);
      analogWrite(m12, 0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright/2);
  }}
  void rotateLeft(){
      analogWrite(m11,speedleft/2);
      analogWrite(m12, 0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright/2);
  }
  
  void turnRight(){
  
    analogWrite(m11,0);
    analogWrite(m12, speedleft/2);
    analogWrite(m21, 0);
    analogWrite(m22, 0);
  }
  void rotateRight(){
  while(ledStates[center])
   {analogWrite(m11,0);
    analogWrite(m12, speedleft/2);
    analogWrite(m21, speedright/2);
    analogWrite(m22, 0);
  }
  }
  void uTurn() {
    analogWrite(m11,0);
    analogWrite(m12, speedleft);
    analogWrite(m21, speedright);
    analogWrite(m22, 0);
    
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
      analogWrite(m11,speedleft/3);
    analogWrite(m12, 0);
    analogWrite(m21, speedright/3);
    analogWrite(m22, 0);
    
    }
 
  

  void setup() {
  
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
      
   if(ledStates[centreFront]&& ledStates[center] && !ledStates[leftFar] && !ledStates[leftNear] && !ledStates[rightFar] && !ledStates[rightNear]){
      goStraight();//1
    }
    else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && ledStates[rightNear] && ledStates[rightFar] && ledStates[center]){
     turnRight();//2
    }
    else if(ledStates[center] && ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && !ledStates[rightNear] && !ledStates[rightFar]){
      rotateLeft();//3
    }
    else if(ledStates[center] && ledStates[centreFront] && ledStates[leftNear] && !ledStates[leftFar] && !ledStates[rightNear] && !ledStates[rightFar]  ){
      goStraight();//4
    }
    else if(ledStates[center] && ledStates[centreFront] && ledStates[rightNear]&& !ledStates[leftFar] && !ledStates[leftNear] && !ledStates[rightFar] ){
      goStraight();//5
    }
    else if(ledStates[leftNear] && ledStates[center] && !ledStates[leftFar] && !ledStates[centreFront] && !ledStates[rightNear] && !ledStates[rightFar]){
      rotateLeft();//6
    }
    else if(ledStates[center] && ledStates[rightNear] && !ledStates[rightFar] && !ledStates[centreFront] && !ledStates[leftNear] && !ledStates[leftFar]){
      turnRight();//7
    }
    else if(ledStates[leftFar] && !ledStates[center] && !ledStates[leftNear] && !ledStates[centreFront] && !ledStates[rightNear] && !ledStates[rightFar]){
       rotateLeft();//8
     }
    else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && !ledStates[rightNear] && ledStates[rightFar]){
       
          turnRight();//9     
    }
     else if(ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && ledStates[rightNear] && !ledStates[rightFar] && ledStates[center]){
     rotateLeft(); //10
     }
      else if(!ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && ledStates[rightNear] && ledStates[rightFar] && ledStates[center]){
     turnRight();//11   
     }
      else if(ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && !ledStates[rightNear] && !ledStates[rightFar] && !ledStates[center]){
        rotateLeft();//12
      }
     else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && ledStates[rightNear] && ledStates[rightFar] && !ledStates[center]){ 
       turnRight();//13
      }
     else if(!ledStates[leftFar] && !ledStates[leftNear] && ledStates[centreFront] && !ledStates[rightNear] && ledStates[rightFar] && !ledStates[center]){
      turnRight();//14
     }
     else if(ledStates[leftFar] && !ledStates[leftNear] && ledStates[centreFront] && !ledStates[rightNear] && !ledStates[rightFar] && !ledStates[center]){
      rotateLeft();//15
     }
    else if(!ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && ledStates[rightNear] && !ledStates[rightFar] && ledStates[center]){
     goStraight();// if leftnear and leftFar 0 then left and vice-versa//
      //16&17 
    }
    else if(!ledStates[leftFar] && !ledStates[leftNear] && ledStates[centreFront] && !ledStates[center] && ledStates[rightNear] && ledStates[rightFar]){
      rotateLeft(); //18 
    }
    else if(ledStates[leftFar] && ledStates[leftNear] && ledStates[centreFront] && !ledStates[center] && !ledStates[rightNear] && !ledStates[rightFar] ){
      turnRight();//19
    }
    else if(!ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && ledStates[center] && ledStates[rightNear] && ledStates[rightFar]){
     //condition for T-Junction
     }
     else if(ledStates[leftFar] && ledStates[leftNear] && ledStates[centreFront] && ledStates[center] && ledStates[rightNear] && ledStates[rightFar]){
      //condition for Node
     }
     else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && !ledStates[center] && !ledStates[rightNear] && !ledStates[rightFar]){
      Back();
     }
      else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && !ledStates[center] && ledStates[rightNear] && !ledStates[rightFar]){
      turnRight();
    
    }
     else if(!ledStates[leftFar] && ledStates[leftNear] && !ledStates[centreFront] && !ledStates[center] && !ledStates[rightNear] && !ledStates[rightFar]){
      rotateLeft();

  }
  else if(!ledStates[leftFar] && !ledStates[leftNear] && !ledStates[centreFront] && ledStates[center] && !ledStates[rightNear] && !ledStates[rightFar]){
      goStraight();
  
  }    
  }
  
