  #define leftFar 4 
  #define leftNear 3
  #define centre 2  
  #define rightNear 1   //81   974   975   989   848   989  
  #define rightFar 0    //21   205   148   307   149   399
  int c=1;
  const int numLeds = 6;
  int speedleft=190;
  int speedright=171;
  int thresholds[numLeds] = {621,850,700,850,643};
  int leds[numLeds] = {A0,A1,A2,A3,A4};
  int m11 = 9;  //for left tyre moving backward when high
  int m12 = 3;  //for left tyre moving forward when high
  int m21 = 10; //for right tyre moving backward when high
  int m22 = 11; //for right tyre moving forward when high
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
   Serial.print(c);
   Serial.print("   ");
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
  { while(ledStates[centre]&&ledStates[leftNear]&&ledStates[rightNear])
    {
      analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21,0);
      analogWrite(m22,speedright);
      readLeds();
    } 
  }
 
  void alignright()
  { 
    while(!ledStates[leftNear])
     { /*analogWrite(m11,0);
        analogWrite(m12,speedright-40);
        analogWrite(m21,0);
        analogWrite(m22,speedright-40);
        delay(100); */
        analogWrite(m11,0);
        analogWrite(m12,speedleft);
        analogWrite(m21,0);
        analogWrite(m22,speedright-70);
        if(ledStates[leftFar]|| ledStates[rightFar])
        break; 
        readLeds();
     }
  }
  void specialright()
  {   Serial.print("special right");
      analogWrite(m11,0);
      analogWrite(m12,speedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,speedright-20);
      delay(250);  
      readLeds();
  if(ledStates[centre])
  {
    while(ledStates[centre])
    { analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21,0);
      analogWrite(m22,0);  
      readLeds();
    }
    while(!ledStates[centre])
    { analogWrite(m11,0);
      analogWrite(m12,speedleft);
      analogWrite(m21,0);
      analogWrite(m22,0);
     
      readLeds();  
    }
  }
  else
   {  while(!ledStates[centre])
      { analogWrite(m11,0);
        analogWrite(m12,speedleft);
        analogWrite(m21,0);
        analogWrite(m22,0);
        readLeds();  
      }
      while(ledStates[centre])
      { analogWrite(m11,0);
        analogWrite(m12,speedleft);
        analogWrite(m21,0);
        analogWrite(m22,0);  
        readLeds();
      }
      while(!ledStates[centre])
      { analogWrite(m11,0);
        analogWrite(m12,speedleft);
        analogWrite(m21,0);
        analogWrite(m22,0);
        readLeds();  
      }
   }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
   
  }
  void alignleft()
  { 
    while(!ledStates[rightNear])
    { /* analogWrite(m11,0);
       analogWrite(m12,speedright-40);
       analogWrite(m21,0);
       analogWrite(m22,speedright-40);
       delay(100) ; */
       analogWrite(m11,0);
       analogWrite(m12,speedleft-70);
       analogWrite(m21,0);
       analogWrite(m22,speedright);
   //    if(ledStates[leftFar]|| ledStates[rightFar])
   //     break; 
       readLeds();  
    }
  }
 
  void right()
  {    Serial.print("right");       
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);    
      while(ledStates[centre])
        {  analogWrite(m11,0);
           analogWrite(m12,speedleft-50);
           analogWrite(m21,0);
           analogWrite(m22,speedright-50);    
           
           readLeds();
        }
        while(!ledStates[centre])
         {  analogWrite(m11,0);
            analogWrite(m12,speedleft-40);
            analogWrite(m21,0);
            analogWrite(m22,0);
           
            readLeds();    
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
       
  }
  void left()
  {        analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);    
      while(ledStates[centre])
        {  analogWrite(m11,0);
           analogWrite(m12,speedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,speedright-40);   
          
           readLeds(); 
        }
        while(!ledStates[centre])
         {  analogWrite(m11,0);
            analogWrite(m12,0);
            analogWrite(m21,0);
            analogWrite(m22,speedright-40); 
            readLeds();   
        }
        --c;
  }
  void specialleft()
  { 
    while(ledStates[centre])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,speedright-40);
      
      readLeds();  
    }
    while(!ledStates[centre])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,speedright-40);
     
      readLeds();  
    }   
  }
  void bigstraight()
  {    if(ledStates[centre])
         { if(ledStates[leftNear])
            { if(ledStates[rightNear])
                 straight();
              if(!ledStates[rightNear])
                 alignleft();
            }
           if(ledStates[rightNear])
            { if(!ledStates[leftNear])
                 alignright();
            }
         }
       if(ledStates[leftNear])
         { if(!ledStates[centre])
            { if(!ledStates[rightNear])
               alignleft();
            }
        }
   if(ledStates[rightNear])
   { if(!ledStates[centre])
      { if(!ledStates[leftNear])
          alignright();
      }
   }  
  } 
  void loop()
  {
    readLeds();   
    /* analogWrite(m11,0);
     analogWrite(m12,speedleft);
     analogWrite(m21,0);
     analogWrite(m22,speedright);   
  } 
  
   */if(!ledStates[leftFar]&& !ledStates[rightFar])//aage chalo
     {  bigstraight();
     }
    if(ledStates[leftFar]|| ledStates[rightFar])
    { ++c;
      Serial.print("detected");
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
         { bigstraight();
           readLeds();
         }  
      }
    } 
  }         
