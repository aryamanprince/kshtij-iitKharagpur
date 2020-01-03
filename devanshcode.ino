int Input = 0;
int RightPwr = 0;
int LeftPwr = 0;
int Threshold=50;
double Kp = 0.08;
int P=150;
int t=2000;
int Logic=0;
int c=0;
int extl=A5;
int nearl=A3;
int mid=A2;
int nearr=A1;
int extr=A0;
int kd;

int a(int x)
{
  return analogRead(x);
}
void setup()
{
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(9, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}
void left()
{
       {
       while(analogRead(mid)<900)
       {
        analogWrite(3,0);
        analogWrite(9,20);
        analogWrite(11,100); 
       }
       while(analogRead(extl)<750||analogRead(mid)>520)
       {
        analogWrite(3,0);
        analogWrite(9,20);
        analogWrite(11,90); 
       }
}
}
void right()
{
       {
       while(analogRead(mid)<800)
       {
        analogWrite(11,0);
        analogWrite(10,20);
        analogWrite(3,100);
        analogWrite(9,0); 
       }
       while(analogRead(extr)<730||analogRead(mid)>520)
       {
        analogWrite(11,0);
        analogWrite(10,20);
        analogWrite(3,100);
        analogWrite(9,0); 
       }}
}
void forward()
{
    Input =(analogRead(A3)-analogRead(A1));
    //Serial.println(analogRead(extl));
    //Serial.print("   |   ");
    //if(330>analogRead(A2)>360)
    Kp=(float)15/analogRead(A2);
    if(Kp<0.05)
    {
      Kp=0.05;
    }
    kd=map(analogRead(A2),500,900,140,30);
    //Serial.println(analogRead(A5));
    //else
    //Kp=0.08;
    LeftPwr = (((Input-Threshold) * Kp)+kd);
    RightPwr = (((Threshold - Input) * Kp)+kd);
    analogWrite(9, 0);
    analogWrite(3, LeftPwr);
    analogWrite(10, 0);
    analogWrite(11, RightPwr);
}
void count()
{
    while((analogRead(extl)<750||analogRead(extr)<750))
      {
        analogWrite(3,40);
        analogWrite(11,40);
       }
       c++;
}
void loop()
{
    forward();
    if(analogRead(extl)<100||analogRead(extr)<100)
    {
      count();
    }
    if(analogRead(extl<100&&analogRead(mid)<550))
    if(c==11||c==12||c==23||c==24)
    {
      left();
       //Serial.println("Stuck2");
     }
     if(analogRead(extr)>600&&analogRead(mid)>550)
     {
      if(c==5||c==6||c==17||c==18||c==29||c==30)
    {
      right();
      // Serial.println("Stuck2");
    }
     }
    //Serial.println(c);
    if(c==35)
    {
      delay(1000000);
    }
}
