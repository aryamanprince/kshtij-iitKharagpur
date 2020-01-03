#define leftFarSensor      3

#define leftNearSensor     4

#define CenterSensor       5

#define rightNearSensor    6

#define rightFarSensor     7

int leftNearReading;
int box=2;
int leftFarReading;

int CenterReading;

int rightNearReading;

int rightFarReading;



#define leapTime 200

#define leftMotor1  8

#define leftMotor2  9

#define rightMotor1 10

#define rightMotor2 11





int redout=0,greenout=12,yellowout=1;
int red1=A1,green1=A2,yellow1=A3;
int colour1,colour2,colour3,c=0;
void setup()

{

  pinMode(leftNearSensor, INPUT);

  pinMode(leftFarSensor, INPUT);

  pinMode(CenterSensor, INPUT);

  pinMode(rightNearSensor, INPUT);
pinMode(A0,INPUT);
  pinMode(rightFarSensor, INPUT);
    pinMode(box,INPUT);
  pinMode(greenout, OUTPUT);
  pinMode(redout, OUTPUT);
  pinMode(green1, OUTPUT);
  pinMode(red1, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
pinMode(yellow1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);

  pinMode(rightMotor1, OUTPUT);

  pinMode(rightMotor2, OUTPUT);






  //Serial.begin(115200);


  delay(1000);

}

void loop()

{
 
 readSensors();
                                                                                     

 if(leftFarReading == HIGH && rightFarReading == HIGH &&

   (CenterReading == LOW || leftNearReading == LOW) )

   {
 c++;
  digitalWrite(redout,HIGH);
  delay(40);
  int red =analogRead(A0);
   delay(10);
  digitalWrite(redout,LOW);

   digitalWrite(greenout,HIGH);
  delay(40);
  int green=analogRead(A0);
   delay(10);
  digitalWrite(greenout,LOW);
 

 
   digitalWrite(yellowout,HIGH);
  delay(40);
  int yellow=analogRead(A0);
  delay(10);
  digitalWrite(yellowout,LOW);


  if(c==1)
  { if(red>yellow&&red>green)
      colour1=red1;
   else if(yellow>red&&yellow>green)
       colour1=yellow1;
   else if(green>yellow&&green>red)
       colour1=green1;
  }
  else if(c==2)
   { if(red>yellow&&red>green)
      colour2=red1;
   else if(yellow>red&&yellow>green)
       colour2=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
  else
   { if(red>yellow&&red>green)
      colour3=red1;
   else if(yellow>red&&yellow>green)
       colour3=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
    straight();                                                                                      

  }
 

  else

  {                                                                                              
 c++;
  digitalWrite(redout,HIGH);
  delay(40);
  int red =analogRead(A0);
   delay(10);
  digitalWrite(redout,LOW);

   digitalWrite(greenout,HIGH);
  delay(40);
  int green=analogRead(A0);
   delay(10);
  digitalWrite(greenout,LOW);
 

 
   digitalWrite(yellowout,HIGH);
  delay(40);
  int yellow=analogRead(A0);
  delay(10);
  digitalWrite(yellowout,LOW);


  if(c==1)
  { if(red>yellow&&red>green)
      colour1=red1;
   else if(yellow>red&&yellow>green)
       colour1=yellow1;
   else if(green>yellow&&green>red)
       colour1=green1;
  }
  else if(c==2)
   { if(red>yellow&&red>green)
      colour2=red1;
   else if(yellow>red&&yellow>green)
       colour2=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
  else
   { if(red>yellow&&red>green)
      colour3=red1;
   else if(yellow>red&&yellow>green)
       colour3=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
    leftHandWall();                                                                                  

  }

}

void readSensors()

{

  leftNearReading    = digitalRead(leftNearSensor);

  leftFarReading     = digitalRead(leftFarSensor);

  CenterReading      = digitalRead(CenterSensor);

  rightNearReading   = digitalRead(rightNearSensor);

  rightFarReading    = digitalRead(rightFarSensor);  

// serial printing below for debugging purposes

// Serial.print("leftCenterReading: ");

// Serial.println(leftCenterReading);

// Serial.print("leftNearReading: ");

// Serial.println(leftNearReading);

// Serial.print("leftFarReading: ");

// Serial.println(leftFarReading);

 

// Serial.print("rightCenterReading: ");

// Serial.println(rightCenterReading);

// Serial.print("rightNearReading: ");

// Serial.println(rightNearReading);

// Serial.print("rightFarReading: ");

// Serial.println(rightFarReading);

// delay(200);  

}

void leftHandWall()

{
 

  if(leftFarReading == LOW && rightFarReading == LOW){

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(450);

    readSensors();

 
    if(leftFarReading == HIGH && rightFarReading == HIGH)

    {

       turnLeft();

    }

    else

    {

       done();

    }

  }

  if(leftFarReading == LOW)

  {

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(450);

    readSensors();

      if(leftFarReading == HIGH && rightFarReading == HIGH)

      {

        turnLeft();

      }

      else

      {

        done();

      }

  }

 

  if(rightFarReading == LOW)

  {

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(450);

    readSensors();

   

     if(leftFarReading == LOW)

     {

     delay(leapTime-30);

     readSensors();

     

      if(rightFarReading == LOW && leftFarReading == LOW)

      {

        done();

      }

      else if(rightFarReading == HIGH && leftFarReading == HIGH && CenterReading == HIGH && rightNearReading == HIGH)

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

    readSensors();

    if(leftFarReading == HIGH && CenterReading == HIGH &&

      rightFarReading == HIGH){

     turnRight();

      return;

    }

   

    straight();

  }

  readSensors();

  if((leftFarReading == HIGH && CenterReading == HIGH

    && rightFarReading == HIGH && leftNearReading == HIGH && rightNearReading == HIGH)||(digitalRead(box)==HIGH&&leftFarReading == HIGH&&rightFarReading == HIGH && leftNearReading == HIGH && rightNearReading == HIGH))

    {

    turnAround();

    }

}

void done()

{

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

 analogWrite(colour1,1023);
    delay(400);
    analogWrite(colour1,0);
   
    analogWrite(colour2,1023);
    delay(400);
    analogWrite(colour2,0);

   analogWrite(colour3,1023);
    delay(400);
    analogWrite(colour3,0);

}

void turnLeft()

{

  while(digitalRead(leftNearSensor) == HIGH)

  {

     digitalWrite(leftMotor1,  LOW);

     digitalWrite(leftMotor2,  LOW);

     digitalWrite(rightMotor1, HIGH);

     digitalWrite(rightMotor2, LOW);

     delay(2);

     digitalWrite(leftMotor1,  LOW);

     digitalWrite(leftMotor2,  LOW);

     digitalWrite(rightMotor1, LOW);

     digitalWrite(rightMotor2, LOW);

     delay(1);

  }

     

   while(digitalRead(leftNearSensor) == HIGH)

   {

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(2);

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

  }

  while(digitalRead(leftFarSensor) == LOW && digitalRead(leftNearSensor) == LOW)

  {

     digitalWrite(leftMotor1,  LOW);

     digitalWrite(leftMotor2,  LOW);

     digitalWrite(rightMotor1, HIGH);

     digitalWrite(rightMotor2, LOW);

     delay(2);

     digitalWrite(leftMotor1,  LOW);

     digitalWrite(leftMotor2,  LOW);

     digitalWrite(rightMotor1, LOW);

     digitalWrite(rightMotor2, LOW);

     delay(1);

  }

 



}

void turnRight()

{

  while(digitalRead(rightNearSensor) == LOW){

     digitalWrite(leftMotor1, HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

     digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

  }

   while(digitalRead(rightNearSensor) == HIGH){

     digitalWrite(leftMotor1, HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

     digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

  }

   while(digitalRead(leftNearSensor) == HIGH){

     digitalWrite(leftMotor1, HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

  }

 

 

}

void straight(){

 
  if( digitalRead(leftNearSensor) == HIGH){

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    return;

  }

 

  if(digitalRead(rightNearSensor) == HIGH){

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    return;

  }

 

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

    digitalWrite(leftMotor1,  LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);
 

}

void turnAround(){

    digitalWrite(leftMotor1,  HIGH);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(450);

   while(digitalRead(leftNearSensor) == HIGH || digitalRead(CenterSensor) == HIGH){

     digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2,  HIGH);

    digitalWrite(rightMotor1, HIGH);

    digitalWrite(rightMotor2, LOW);

    delay(1);

     digitalWrite(leftMotor1, LOW);

    digitalWrite(leftMotor2,  LOW);

    digitalWrite(rightMotor1, LOW);

    digitalWrite(rightMotor2, LOW);

    delay(1);

  }


}



void endMotion()

{

  endMotion();

}

void colourSensor(){
  c++;
  digitalWrite(redout,HIGH);
  delay(40);
  int red =analogRead(A0);
   delay(10);
  digitalWrite(redout,LOW);

   digitalWrite(greenout,HIGH);
  delay(40);
  int green=analogRead(A0);
   delay(10);
  digitalWrite(greenout,LOW);
 

 
   digitalWrite(yellowout,HIGH);
  delay(40);
  int yellow=analogRead(A0);
  delay(10);
  digitalWrite(yellowout,LOW);


  if(c==1)
  { if(red>yellow&&red>green)
      colour1=red1;
   else if(yellow>red&&yellow>green)
       colour1=yellow1;
   else if(green>yellow&&green>red)
       colour1=green1;
  }
  else if(c==2)
   { if(red>yellow&&red>green)
      colour2=red1;
   else if(yellow>red&&yellow>green)
       colour2=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
  else
   { if(red>yellow&&red>green)
      colour3=red1;
   else if(yellow>red&&yellow>green)
       colour3=yellow1;
   else if(green>yellow&&green>red)
       colour3=green1;
  }
 
  }
