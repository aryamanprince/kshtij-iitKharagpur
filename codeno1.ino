  #define leftFar 4 
  #define leftNear 3
  #define centre 2  
  #define rightNear 1   //81   974   975   989   848   989  
  #define rightFar 0    //21   205   148   307   149   399
  int c=1;
  const int numLeds = 6;
  int speedleft=163;
  int speedright=140;
  int thresholds[numLeds] = {621,624,663,605,643};
  int leds[numLeds] = {A4,A3,A2,A1,A0};
  int leftMotor2 = 9;  //for left tyre moving backward when 1
  int leftMotor1 = 3;  //for left tyre moving forward when 1
  int rightMotor2 = 10; //for right tyre moving backward when 1
  int rightMotor1 = 11; //for right tyre moving forward when 1
  int ledStates[numLeds]={};
  
  #define leapTime 200
  
  void setup()
  {
    Serial.begin(9600);
    
    pinMode(leftMotor1,OUTPUT);
    pinMode(leftMotor2,OUTPUT);
    pinMode(rightMotor1,OUTPUT);
    pinMode(rightMotor2,OUTPUT);
  
    for(int i=0;i < numLeds;i++)
      pinMode(leds[i],INPUT);
    
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
   delay(100);
   Serial.println();
  }  
void loop()

{ /*  analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);
}*/
 
 readLeds();
                                                                                     

 if(!ledStates[leftFar]&& !ledStates[rightFar] &&

   (ledStates[centre] || ledStates[leftNear] ) )

   {
 
    straight();                                                                                      

   }
 

  else

  {                                                                                              
 
    leftHandWall();                                                                                  

  }

}

void readLeds()
  {
    for(int i=0;i < numLeds;i++)
      ledStates[i] = analogRead(leds[i]);
      
    printLeds();
    putThreshold();
    printLeds();
  }
 
void leftHandWall()

{
 

  if(ledStates[leftFar] && ledStates[rightFar]){

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(450);

    readLeds();

 
    if(!ledStates[leftFar]&& !ledStates[rightFar])

    {

       turnLeft();

    }

    else

    {

       done();

    }

  }

  if(ledStates[leftFar])

  {

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(450);

    readLeds();

      if(!ledStates[leftFar] && !ledStates[rightFar])

      {

        turnLeft();

      }

      else

      {

        done();

      }

  }

 

  if(ledStates[rightFar])

  {

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(450);

    readLeds();

   

     if(ledStates[leftFar])

     {

     delay(leapTime-30);

     readLeds();

     

      if(ledStates[rightFar]&& ledStates[leftFar])

      {

        done();

      }

      else if(!ledStates[rightFar] && !ledStates[leftFar] && !ledStates[centre]  && !ledStates[rightNear] )

      {

        turnRight();

      }

      else

      {

        straight();

        return;

      }

   }

    delay(leapTime-30);

    readLeds();

    if(!ledStates[leftFar] && !ledStates[centre] && !ledStates[rightFar] )
    {

     turnRight();

      return;

    }

   

    straight();

  }

  readLeds();

  if((!ledStates[leftFar]&& !ledStates[centre]&& !ledStates[rightFar]&&!ledStates[leftNear]&&!ledStates[rightNear])
  ||(!ledStates[leftFar]&&!ledStates[rightFar] && !ledStates[leftNear]&& !ledStates[rightNear]))

    {

    turnAround();

    }

}

void done()

{

    straight();

}

void turnLeft()

{

  while(!ledStates[leftNear])

  {

     analogWrite(leftMotor1,0);

     analogWrite(leftMotor2,0);

     analogWrite(rightMotor1,speedright);

     analogWrite(rightMotor2,0);

     delay(2);

     analogWrite(leftMotor1,0);

     analogWrite(leftMotor2,0);

     analogWrite(rightMotor1,0);

     analogWrite(rightMotor2,0);

     delay(1);

  }
   

  while(ledStates[leftNear]&& ledStates[leftFar])

  {

     analogWrite(leftMotor1,0);

     analogWrite(leftMotor2,0);

     analogWrite(rightMotor1,speedright);

     analogWrite(rightMotor2,0);

     delay(2);

     analogWrite(leftMotor1,0);

     analogWrite(leftMotor2,0);

     analogWrite(rightMotor1,0);

     analogWrite(rightMotor2,0);

     delay(1);

  }

 



}

void turnRight()

{

  while(ledStates[rightNear]){

     analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2,0);

    delay(1);

     analogWrite(leftMotor1,0);

    analogWrite(leftMotor2, 0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2,0);

    delay(1);

  }

   while(!ledStates[rightNear]){

     analogWrite(leftMotor1, speedleft);

    analogWrite(leftMotor2, 0);

    analogWrite(rightMotor1, 0);

    analogWrite(rightMotor2, 0);

    delay(1);

     analogWrite(leftMotor1,0);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2,0);

    delay(1);

  }

   while(!ledStates[leftNear]){

     analogWrite(leftMotor1, speedleft);

    analogWrite(leftMotor2, 0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2,0);

    delay(1);

    analogWrite(leftMotor1,0);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2,0);

    delay(1);

  }

 

 

}

void straight(){

 
  if(!ledStates[leftNear]){

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(1);

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,0);

    analogWrite(rightMotor2, 0);

    delay(1);

    return;

  }

 

  if(!ledStates[rightNear]){

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(1);

    analogWrite(leftMotor1,0);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(1);

    return;

  }

 

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2,0);

    delay(1);

    analogWrite(leftMotor1,  0);

    analogWrite(leftMotor2,  0);

    analogWrite(rightMotor1, 0);

    analogWrite(rightMotor2, 0);

    delay(1);
 

}

void turnAround(){

    analogWrite(leftMotor1,speedleft);

    analogWrite(leftMotor2,  0);

    analogWrite(rightMotor1,speedright);

    analogWrite(rightMotor2, 0);

    delay(450);

   while(!ledStates[leftNear] || !ledStates[centre]){

     analogWrite(leftMotor1, 0);

    analogWrite(leftMotor2, speedleft);

    analogWrite(rightMotor1, speedright);

    analogWrite(rightMotor2, 0);

    delay(1);

     analogWrite(leftMotor1, 0);

    analogWrite(leftMotor2,  0);

    analogWrite(rightMotor1, 0);

    analogWrite(rightMotor2, 0);

    delay(1);

  }


}



void endMotion()

{

  endMotion();

}
