  #define leftFar 0 
  #define leftNear 1
  #define centre 2  
  #define rightNear 3   //109   158   205   120   109   104 
  #define rightFar 4    //530   793   962   722   618   424
  #define centreFront 5
  
  const int numLeds = 6;
  int speedleft=117;
  int speedright=110;
  int thresholds[numLeds] = {337,415,576,153,317,189};
  int leds[numLeds] = {A0,A1,A2,A3,A4,A5};
  int m11 = 6;  //for left tyre moving backward when high
  int m12 = 5;    //for left tyre moving forward when high
  int m21 = 11;    //for right tyre moving backward when high
  int m22 = 10;   //for right tyre moving forward when high
  int ledStates[numLeds] = {};
  int sum=0;
  int sumleds[numLeds]={1,2,4,8,16,32};
  void readLeds(){
    sum=0;
    for(int i=0;i < numLeds;i++)
      ledStates[i] = analogRead(leds[i]);
      
    printLeds();
    putThreshold();
    printLeds();
  }
  void printLeds(){
   Serial.print(ledStates[leftFar]);
   Serial.print("   ");
   Serial.print(ledStates[leftNear]);
   Serial.print("   ");
   Serial.print(ledStates[centre]);
   Serial.print("   ");
   Serial.print(ledStates[rightNear]);
   Serial.print("   ");
   Serial.print(ledStates[rightFar]);
   Serial.print("   ");
   Serial.print(ledStates[centreFront]);
   Serial.print("   ");
   delay(200);
   Serial.println();
  }
  void left()
  {   analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright);
     
    
    while(ledStates[centre]&& ledStates[centreFront])// till both centre leds only on line
     {   
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright);
      readLeds();
     }
  }
  void right()
  {   analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21, 0);
      analogWrite(m22,0);
      
      
     while(ledStates[centre]&& ledStates[centreFront])// till both centre leds only on line
      {analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21, 0);
      analogWrite(m22,0);
      readLeds();
      }
  }
  void slightleft(){
     
     while(ledStates[centre]&& ledStates[centreFront])// till both centre leds only on line
     {   
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21, 0);
      analogWrite(m22,speedright);
      readLeds();
     }

    
  }

  void slightright(){
      while(ledStates[centre]&& ledStates[centreFront])// till both centre leds only on line
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
  void straight(){
    
    analogWrite(m11,0);
    analogWrite(m12,speedleft);
    analogWrite(m21,0);
    analogWrite(m22,speedright);
    
    
  }
  
  void putThreshold(){
    // Thresholding to set the proper values of led states
    for(int i=0;i < numLeds;i++)
    {
      if(ledStates[i] > thresholds[i])
      {
        ledStates[i] = 1;
        sum+=sumleds[i];
      }
      
      else
      {
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
    void back()
    { 
      analogWrite(m11,speedleft-50);
      analogWrite(m12, 0);
      analogWrite(m21, speedright-50);
      analogWrite(m22, 0);
      if(ledStates[rightFar])
      right();
      else if(ledStates[leftFar])
      left();
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
       switch(sum)
    { // straight motion
      case 36: straight();break;
      //case 4: straight();break;
      case 38: slightleft();break;
      case 6: slightleft();break;
      case 44: slightright();break;
      case 12: slightright();break;
      case 46: straight();break;
     // left turn
      case 1: left();break;
      case 3: left();break;
      case 7: left();break;
      case 15: left();break;

     // right turn
     case 16: right();break;
     case 24: right();break;
     case 28: right();break;
     case 30: right();break;

     // t joint
     case 60: right();break;
     case 37: left();break;
     case 31: right;break;
     // cross
     case 63: left();break;
     case 0: right();break;
    }
  }
