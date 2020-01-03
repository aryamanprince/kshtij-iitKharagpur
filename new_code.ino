#define leftFar 4
#define leftNear 3
#define rightNear 1
#define rightFar 0
#define centre 2
#define centreFront 5
//564 974 984 987 975 992
//54 223 148 289 132 420
const int numLeds = 6;
int maxspeedleft =175;//163
int maxspeedright = 160;//140
int c = 1;
long leds[numLeds] = {A4, A3, A2, A1, A0, A5};
int thresholds[numLeds] = {51,589,561,648,498,702};
int m11 = 10;  //for right tyre moving backward when high
int m12 = 6;    //for left tyre moving forward when high
int m21 =5;    //for left tyre moving backward when high
int m22 = 11;   //for right tyre moving forward when high
long ledStates[numLeds] = {};
int TledStates[numLeds] = {};
long ledAverage;
long ledSum;
long botPosition;
int proportional;
int previousproportional;
int derivative;
int error;
int nodeCount=1;
double kp = 0.21;
double kd = 0;
int setPoint = 1900;
int rightSpeed;
int leftSpeed;
int noderightspeed=0;
int nodeleftspeed=0;
int turningSpeed=120;
void specialright()
  { c--;
    while(ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,0);  
      Serial.print(c);
      Serial.println();
      read_leds();
    }
    while(!ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,0);
      Serial.print(c);
      Serial.println();
      read_leds();  
    }   
  }
 void specialleft()
  {  c--;
    while(ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);
      Serial.print(c);
      Serial.println();
      read_leds();  
    }
    while(!ledStates[centreFront])
    { analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);
      Serial.print(c);
      Serial.println();
      read_leds();  
    }   
  }

void left()
  { c--;
      while(ledStates[centreFront])
        {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);   
           Serial.print(c);
           Serial.println();
           read_leds(); 
        }
        while(!ledStates[centreFront])
         {  analogWrite(m11,0);
            analogWrite(m12,0);
            analogWrite(m21,0);
            analogWrite(m22,maxspeedright-40); 
            read_leds();   
        }
  }
void right()
  {   c--; 
      while(ledStates[centreFront])
        {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);    
           Serial.print(c);
           Serial.println();
           read_leds();
        }
        while(!ledStates[centreFront])
         {  analogWrite(m11,0);
            analogWrite(m12,maxspeedleft-40);
            analogWrite(m21,0);
            analogWrite(m22,0);
            Serial.print(c);
            Serial.println();
            read_leds();    
        }
  }
 

void checkTurn() {
 if(!ledStates[leftFar]&&!ledStates[rightFar])
    { read_leds();
    pidCalc();
    errorAdj();
    motorRun();
    } 
  else {
    c++;
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
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);
           Serial.print(c);
           Serial.println();
           read_leds();
         }  
      }
  }

}
void putThreshold() {
  // Thresholding to set the proper values of led states
  for (int i = 0; i < numLeds; i++) {
    if (ledStates[i] > thresholds[i]) {
      TledStates[i] = 1;
    }

    else {
      TledStates[i] = 0;
    }
  }
}
void pidCalc() {

  proportional = botPosition - setPoint ;

  previousproportional = proportional;
  derivative = proportional - previousproportional;
  error = (proportional * kp) + (derivative * kd);
}
void errorAdj()
{
  if (error < -160)
  {
    error = -160;
  }
  if (error > 160)
  {
    error = 160;
  }
  // If error_value is less than zero calculate right turn speed values
  if (error < 0)
  {
    rightSpeed = maxspeedright + error;
    leftSpeed = maxspeedleft;
  }
  // If error_value is greater than zero calculate left turn values
  else
  { rightSpeed = maxspeedright;
    leftSpeed = maxspeedleft - error;
  }
}
void motorRun()
{ // Drive motors according to the calculated values for a turn
  analogWrite(m12, rightSpeed);
  analogWrite(m22, leftSpeed);
  
}
void read_leds() {
  ledAverage = 0;
  ledSum = 0;
ledStates[0]=analogRead(leds[0]);
  Serial.print(ledStates[0]);
  Serial.print(" ");
  for (int i = 1 ; i < 4; i++) {
    ledStates[i] = analogRead(leds[i]);
    ledAverage += ledStates[i] * i * 1000;
    ledSum += ledStates[i];
    Serial.print(ledStates[i]);
    Serial.print(" ");
  }
  ledStates[4]=analogRead(leds[4]);
  Serial.print(ledStates[4]);
  Serial.print(" ");
  ledStates[5]=analogRead(leds[5]);
  Serial.print(ledStates[5]);
  Serial.print(" ");
  putThreshold();
  botPosition = ledAverage / ledSum;
  Serial.print(ledAverage);
  Serial.print(" ");
  Serial.print(ledSum);
  Serial.print(" ");
  Serial.print(botPosition);
  Serial.println(" ");
  for(int i=0;i<numLeds;i++){
    Serial.print(TledStates[i]);
    Serial.print("");
    }
    
    Serial.println(" ");
   

}


void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(A5, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A0, INPUT);

}


void loop() {
  read_leds();
  checkTurn();
 /*if(f=1){
    checkTurnSpecial();
    }*/
     //pidCalc();
    //errorAdj();
    //motorRun();
}
