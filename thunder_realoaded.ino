  #define leftFar 4 
  #define leftNear 3
  #define centre 2  
  #define rightNear 1   //652   978   983   985   960  992  
  #define rightFar 0    //64   209   142   310   127   413 
  #define centreFront 5
  int c=1;
  const int numLeds = 6;
  int speedleft=163;
  int speedright=140;
  int thresholds[numLeds] = {543,647,562,593,358,702};
  int leds[numLeds] = {A0,A1,A2,A3,A4,A5};
  int m11 = 10;  //for left tyre moving backward when high
  int m12 = 11;  //for left tyre moving forward when high
  int m21 = 5; //for right tyre moving backward when high
  int m22 = 6; //for right tyre moving forward when high
  int ledStates[numLeds] = {};
   
   void setup()
  {
    Serial.begin(9600);
    
    pinMode(m11,OUTPUT);
    pinMode(m12,OUTPUT);
    pinMode(m21,OUTPUT);
    pinMode(m22,OUTPUT);
  
    for(int i=0;i < numLeds;i++)
      pinMode(leds[i],INPUT);
    
  }
   
  void readLeds()
  {
    for(int i=0;i < numLeds;i++)
      ledStates[i] = analogRead(leds[i]);
      
    printLeds();
    putThreshold();
    printLeds();
  }
  void printLeds()
  {
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
   delay(100);
   Serial.println();
  }
  void putThreshold()
  {
    // Thresholding to set the proper values of led states
    for(int i=0;i < numLeds;i++)
    {
      if(ledStates[i] > thresholds[i])
      {
        ledStates[i] = 1;
      }
      
      else
      {
        ledStates[i] = 0;
      }
    }
  }
  void straight()
  { while(ledStates[centre]&&ledStates[leftNear])
    {
      analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21,0);
      analogWrite(m22,speedright);
      Serial.print(c);
      Serial.println();
      readLeds();
    } 
  }
  
  void alignright()
  { while(!ledStates[leftNear])
     {  
        analogWrite(m11,0);
        analogWrite(m12,speedleft);
        analogWrite(m21,0);
        analogWrite(m22,0);
        Serial.print(c);
        Serial.println();
        readLeds();
     }
  }
  void specialright()
  { c--;
    while(ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,speedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,0);  
      Serial.print(c);
      Serial.println();
      readLeds();
    }
    while(!ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,speedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,0);
      Serial.print(c);
      Serial.println();
      readLeds();  
    }   
  }
  void alignleft()
  { while(!ledStates[centre])
    {   
       analogWrite(m11,0);
       analogWrite(m12,0);
       analogWrite(m21,0);
       analogWrite(m22,speedright);
        
       Serial.print(c);
       Serial.println();
       readLeds();  
    }
  }
  void alignright2()
  {  while(ledStates[rightNear])
      {    analogWrite(m11,0);
           analogWrite(m12,speedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,0);   
      }
  }
  void right()
  {   c--; 
      while(ledStates[centreFront])
        {  analogWrite(m11,0);
           analogWrite(m12,speedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,speedright-40);    
           Serial.print(c);
           Serial.println();
           readLeds();
        }
        while(!ledStates[centreFront])
         {  analogWrite(m11,0);
            analogWrite(m12,speedleft-40);
            analogWrite(m21,0);
            analogWrite(m22,0);
            Serial.print(c);
            Serial.println();
            readLeds();    
        }
  }
  void left()
  { c--;
      while(ledStates[centreFront])
        {  analogWrite(m11,0);
           analogWrite(m12,speedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,speedright-40);   
           Serial.print(c);
           Serial.println();
           readLeds(); 
        }
        while(!ledStates[centreFront])
         {  analogWrite(m11,0);
            analogWrite(m12,0);
            analogWrite(m21,0);
            analogWrite(m22,speedright-40); 
            readLeds();   
        }
  }
  void specialleft()
  {  c--;
    while(ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,speedright-40);
      Serial.print(c);
      Serial.println();
      readLeds();  
    }
    while(!ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,speedright-40);
      Serial.print(c);
      Serial.println();
      readLeds();  
    }   
  }
    
  void loop()
  { Serial.print(c);
    Serial.println();
    readLeds();
  /*   analogWrite(m11,0);
     analogWrite(m12,speedleft);
     analogWrite(m21,0);
     analogWrite(m22,speedright);   
  } */
  
    if(!ledStates[leftFar]&& !ledStates[rightFar])//aage chalo
     {  if(ledStates[centre])
        {
          if(ledStates[leftNear])
             straight();
          if(!ledStates[leftNear])
             alignright();
          else if(ledStates[rightNear])
             alignright2();
        }
        if(ledStates[leftNear])
        { 
          if(!ledStates[centre])
             alignleft();
        }
        if(ledStates[rightNear])
        { if(!ledStates[centre])
            alignright2();
        }       
     }
    if(ledStates[leftFar]|| ledStates[rightFar])
    { c++;
      if(c==6 || c==18 || c==30 || c==31)
       right();
      else if(c==7 || c==19)
       specialright();
      else if(c==12 || c==24)
       left();
      else if(c==13 || c==25)
       specialleft();
      else if(c==36)
      { analogWrite(m11,0);
        analogWrite(m12,0);
        analogWrite(m21,0);
        analogWrite(m22,0);
      }
      else
      { while(ledStates[leftFar]|| ledStates[rightFar])
         { analogWrite(m11,0);
           analogWrite(m12,speedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,speedright-40);
           Serial.print(c);
           Serial.println();
           readLeds();
         }  
      }
    } 
  }           
