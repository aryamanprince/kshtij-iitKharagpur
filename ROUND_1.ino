#define leftFar 0//264 330 446 325 304 
#define leftNear 1//967 973 971 965 962
#define rightNear 3
#define rightFar 4
#define center 2
//#define centerFront 5
//270 273 352 240 315
//973 976 975 970 971
#include<LiquidCrystal.h>
LiquidCrystal lcd(A3,A2, 7, 8, 4, 2);
#include <SPI.h>
#include <MFRC522.h>

const int numLeds = 5;
int maxspeedleft =110;//107;//163//140//130
int maxspeedright =114;//140;//140//151//140
int c  = 1,acount=0;
long leds[numLeds] = {A0,A1,A5,A6,A7};
int thresholds[numLeds] = {621,624,663,605,643};
int m11 = 3;  //for left tyre moving backward when high
int m12 = 9;    //for left tyre moving forward when high
int m21 =6;    //for right tyre moving backward when high
int m22 = 5;   //for right tyre moving forward when high
long ledStates[numLeds] = {};
int TledStates[numLeds] = {};
long ledAverage;
long ledSum;
int rightSpeed;
int leftSpeed;
long botPosition;
int proportional;
int previousproportional;
int derivative;
int error;
double kp=0.15;//0.1;//0.1;//0.17;
double kd =0.18;//((0.17;)5/5)//((0.16;)5 out of 6 )//((0.15;))//(0.14;)//(0.13;)//(0.12;)//(0.11;)//(0.1;)//0.52;//0.5;//.3;
int setPoint =0;
int l=0,g=0,h=0; //counter for rfid storage 
#define RST_PIN         4           // Configurable, see typical pin layout above
#define SS_IN          10          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_IN, RST_PIN);   // Create MFRC522 instance
int y=0,z=0;
int rows= 5;
byte input[5][5] = {{0}};
int result[5][5] ={{0}};
int visited[5][5]={{0}};
int queue[2][36]={{0}};    
int previousResult[5][5];
int nodesSolved = 0;
int nodes;
int b;
int calcSafe(int ,int);
void newClue (int, int);
void copyArray(int array[5][5],int copy[5][5]);
int checkAssumption();
void decipherPoint(int,int);
int noProgress(int array1[5][5],int array2[5][5]);
void popBack(int *i,int *j){
        int a;
    for(a=0;queue[0][a]!=0||queue[1][a]!=0;a++){}
     queue[0][a-1]=0;
     queue[1][a-1]=0;
     *i=queue[0][a-2];
     *j=queue[1][a-2];
}
void pushToQueue(int i,int j ){
        int a;
for( a=0;queue[1][a]!=0||queue[0][a]!=0;a++){}
queue[0][a]=i;
queue[1][a]=j;
}
void pathPlanning(){
        int i=0;
        int j=0;
        visited[0][0]=1;
while(i!=5-1||j!=5-1){
        if (i < 5 - 1 && result[i + 1][j] == 1&&visited[i+1][j]==0){ 
                i++;
                visited[i][j]=1;
                pushToQueue(i,j);}
           
        else if (j < 5 - 1 && result[i][j+1]==1&&visited[i][j+1]==0 ){
                j++;
                visited[i][j]=1;
                pushToQueue(i,j);}
                                                                                                             
        else if (i >0 && result[i-1][j] == 1&&visited[i-1][j]==0  ){
                i--;
                visited[i][j]=1;
                pushToQueue(i,j);}
        else if (j!=0 && result[i ][j-1] == 1&&visited[i][j-1]==0 ){
                j--;
                visited[i][j]=1;
                pushToQueue(i,j);}
        else{
                popBack(&i,&j);
        }        
    }
        
        
}
void left1()
{  read_leds();
          Serial.print("leftturn");
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);    
           delay(300);
          while(TledStates[center])
           {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);   
           read_leds(); 
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0); 
           delay(100);
        while(!TledStates[center])
         {  analogWrite(m11,maxspeedleft-40);
            analogWrite(m12,0);
            analogWrite(m21,0);
            analogWrite(m22,maxspeedright-40); 
            read_leds();   
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);
          Pstraight();
  }
void assumption(){
int i, j;
   nodes=nodesSolved;
   for( i=0;i<5;i++){
       for( j=0;j<5;j++){
           if(result[i][j]==0)
           break;
       }
       if(result[i][j]==0)
           break;
       
   }
  result[i][j]=1;
  nodesSolved++;
  int safeAssump=checkAssumption();
  nodesSolved=nodes;
  result[i][j]=-1;
  nodesSolved++;
  int ambushAssump=checkAssumption(); 
  nodesSolved=nodes;
  if(safeAssump>ambushAssump){
      result[i][j]=1;
      nodesSolved++;}
  else{ 
      result[i][j]=-1;
      nodesSolved++;}
  
    
}
int checkAssumption(){
    int testArray[5][5]={0};
    copyArray(result,testArray);
     while (nodesSolved != 5*5)  
    {
       copyArray(result,previousResult);
      for (int i = 0; i < 5; i++)
  {
    
    for (int j = 0; j < 5; j++)
      decipherPoint (i, j);
      
     
  } 
     if(noProgress(previousResult,result))
        break;
     
  }
  copyArray(testArray,result);
  return nodesSolved;
}
void copyArray(int array1[5][5] ,int copy[5][5]){
    for(int i=0;i<5;i++){
        for(int j=0;j<5;j++){
            copy[i][j]=array1[i][j];
        } 
    }
}
int noProgress(int array1[5][5],int array2[5][5])
{
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++)
  {
        if(array1[i][j]!=array2[i][j])
            return 0;
        
        }
    }
    
    return 1;   
}
//Sets all unassigned points around a node to ambush
void setAmbush (int i, int j){
  if (i < 5 - 1 && result[i + 1][j] == 0)
    {
      result[i + 1][j] = -1;
      nodesSolved++;
      newClue (i + 1, j);
    }
  if (j < 5 - 1 && result[i][j + 1] == 0)
    {
      result[i][j + 1] = -1;
      nodesSolved++;
      newClue (i, j + 1);
    }
  if (i != 0 && result[i - 1][j] == 0)
    {
      result[i - 1][j] = -1;
      nodesSolved++;
      newClue (i - 1, j);
    }
  if (j != 0 && result[i][j - 1] == 0)
    {
      result[i][j - 1] = -1;
      nodesSolved++;
      newClue (i, j - 1);
    }



}
//Sets all unassigned points around a node to safe
void setSafe (int i, int j){
  if (i < 5 - 1 && result[i + 1][j] == 0)
    {
      result[i + 1][j] = 1;
      nodesSolved++;
      newClue (i + 1, j);
    }
  if (j < 5 - 1 && result[i][j + 1] == 0)
    {
      result[i][j + 1] = 1;
      nodesSolved++;
      newClue (i, j + 1);
    }
  if (i != 0 && result[i - 1][j] == 0)
    {
      result[i - 1][j] = 1;
      nodesSolved++;
      newClue (i - 1, j);
    }
  if (j != 0 && result[i][j - 1] == 0)
    {
      result[i][j - 1] = 1;
      nodesSolved++;
      newClue (i, j - 1);
    }



}
//Returns calculated no of ambush points around a node until now
int calcAmbush (int i, int j){
  int sum = 0;
  if (i < 5 - 1 && result[i + 1][j] == -1)
    sum++;
  if (j < 5 - 1 && result[i][j + 1] == -1)
    sum++;
  if (i != 0 && result[i - 1][j] == -1)
    sum++;
  if (j != 0 && result[i][j - 1] == -1)
    sum++;
  return sum;



}
//Returns calculated no of safe points around a node until now
int calcSafe (int i, int j){
  int sum = 0;
  if (i < 5 - 1 && result[i + 1][j] == 1)
    sum++;
  if (j < 5 - 1 && result[i][j + 1] == 1)
    sum++;
  if (i != 0 && result[i - 1][j] == 1)
    sum++;
  if (j != 0 && result[i][j - 1] == 1)
    sum++;
  return sum;



}
void singleAmbush (int i, int j){
  //Corner
  if ((i == 0 && j == 0) || (i == 5 - 1 && j == 5 - 1)
      || (i == 5 - 1 && j == 0) ||(i == 0 && j == 5 - 1))
    {
      if (calcAmbush (i, j) == 1)
     setSafe (i, j);
    else if (calcSafe (i, j) == 1)
     setAmbush (i, j);
  

    }
//Edge
  else if (i == 0 || j == 0 || i == 5 - 1 || j == 5 - 1)
    {
      if (calcAmbush (i, j) == 1)
      setSafe (i, j);
    else if (calcSafe (i, j) == 2)
      setAmbush (i, j);
    }
//Central
      else
   {
    if (calcAmbush (i, j) == 1)
        setSafe (i, j);
    else if (calcSafe (i, j) == 3) 
        setAmbush (i, j);
    }
}
void doubleAmbush (int i, int j){
//Edge
  if (i == 0 || j == 0 || i == 5 - 1 || j == 5 - 1)
    {
      if (calcAmbush (i, j) == 2)
  {
    setSafe (i, j);
  }
      else if (calcSafe (i, j) == 1)
  {
    setAmbush (i, j);
  }
    }
//Central
  else
    {
      if (calcAmbush (i, j) == 2)
  {
    setSafe (i, j);
  }
      else if (calcSafe (i, j) == 2)
  {
    setAmbush (i, j);
  }



    }


}
void tripleAmbush (int i, int j){

  if (calcAmbush (i, j) == 3)
    {
      setSafe (i, j);
    }
  else if (calcSafe (i, j) == 1)
    {
      setAmbush (i, j);
    }


}
//To check whether nodes around point have already been deciphered,returns 1 if not
int checkPoint (int i, int j){
  if (i < 5 - 1 && result[i + 1][j] == 0)
    return 1;
  if (j < 5 - 1 && result[i][j + 1] == 0)
    return 1;
  if (i > 0 && result[i - 1][j] == 0)
    return 1;
  if (j > 0 && result[i][j - 1] == 0)
    return 1;

  return 0;

}
//Tries dechiphering nodes around a point 
void decipherPoint(int i, int j){
  if (checkPoint (i, j))
    {
      if (input[i][j] == 1)
  singleAmbush (i, j);
      else if (input[i][j] == 2)
  doubleAmbush (i, j);
      else if (input[i][j] == 3)
  tripleAmbush (i, j);
    }


}
//In case a new node is deciphered checks the points around that node
void newClue(int i, int j){
  if (i < 5 - 1)
    decipherPoint (i + 1, j);
  if (j < 5 - 1)
    decipherPoint (i, j + 1);
  if (i != 0)
    decipherPoint (i - 1, j);
  if (j != 0)
    decipherPoint (i, j - 1);
}
//For direct points
void obvious(){
  for (int i = 0; i < 5; i++)
    {
      for (int j = 0; j < 5; j++)
  {
    if (input[i][j] == 0)
      {
        setSafe (i, j);

      }
    if ((input[i][j] == 2)
        && ((i == 0 && j == 5 - 1) || (i == 0 && j == 0)
      || (i == 5 - 1 && j == 0) || (i == 5 - 1
               && j == 5 - 1)))
      {
        setAmbush (i, j);
      }
    if ((input[i][j] == 3)
        && ((i == 0) || (j == 0) || (i == 5 - 1) || (j == 5 - 1)))
      {
        setAmbush (i, j);
      }
    if (input[i][j] == 4)
      {
        setAmbush (i, j);
      }

  }
    }

}

void nodeCount(int*botp,int *botq,int p,int q,int *stopValue){
if(TledStates[rightFar]||TledStates[leftFar])
  { *botp=p;
    *botq=q;
     *stopValue--;
    while(TledStates[rightFar]|| TledStates[leftFar])
      Pstraight();
  }
}     
void increaseDirection(int *directional){
directional++;
if(directional==5)
directional=1;  
}
void decreaseDirection(int *directional){
directional--;
if(directional==-1)
directional=4;  
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
void rfid()
{  Serial.print("rfid");
// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------
//Serial.println("aa");
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
//Serial.println("aa");
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
 
  Serial.println(F("**Card Detected:**"));
 
  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //uncomment this to see all blocks in hex

  //-------------------------------------------

  Serial.print(F("Ambush="));

  byte buffer1[18];

  block = 4;
  len = 18;
/*
  //------------------------------------------- GET FIRST NAME
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid)); //line 834 of MFRC522.cpp file
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer1, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT FIRST NAME
  for (uint8_t i = 0; i < 16; i++)
  {
    if (buffer1[i] != 32)
    {
      Serial.write(buffer1[i]);
    }
  }
  */
  //---------------------------------------- GET LAST NAME

  byte buffer2[18];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

//PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i]);
  }
String abc=(char*)buffer2;
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Ambush=");
  lcd.print(abc);
  lcd.setCursor(0,1);
  lcd.print("c=");
  lcd.print(c);
  lcd.print("   ");
  lcd.print(acount);
  input[g][h]=buffer2[0]-48;
  if(g==0 || g==2 || g==4)
  { h++;
    if(h==5)
    { g++;
      h=4;
    }
   } 
  if(g==1 || g==3)
  { h--;
    if(h==-1)
    { h=0;
      g++;
    }
  }
  if(g==5)
  g=4;
  //----------------------------------------

  Serial.println(F("\n**End Reading**\n"));

  delay(50); //change value if you want to read cards faster

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
void pidCalc() {
 
  proportional = botPosition - setPoint ;
  if(TledStates[center]) //botPosition<-500||botPosition>500)
  kp=0.11;
  else
  kp=0.5;
    derivative = proportional - previousproportional;
  error = (proportional * kp) + (derivative * kd);
  previousproportional = proportional;
}
void errorAdj()
{
 if (error < -140)
  {
    error = -140;
    
  }
  if (error > 140)
  {
    error = 140;
  }
  // If error_value is less than zero calculate right turn speed values
  if (error <0)
  {
    rightSpeed = maxspeedright+error;
    leftSpeed = maxspeedleft;
  }
  // If error_value is greater than zero calculate left turn values
  else
  { rightSpeed = maxspeedright;
    leftSpeed =maxspeedleft-error ;
  }
}
void motorRun()
{ // Drive motors according to the calculated values for a turn
  analogWrite(m22, rightSpeed);
 analogWrite(m12, leftSpeed);
 }
void read_leds() {
  ledAverage = 0;
  ledSum = 0;
  ledStates[0]=analogRead(leds[0]);
  Serial.print(ledStates[0]);
  Serial.print(" ");
  for (int i = 1 ; i < 4; i++) {
    ledStates[i] = analogRead(leds[i]);
    //led Average += ledStates[i] * i * 100;
    //ledSum += ledStates[i];
    Serial.print(ledStates[i]);
    Serial.print(" ");
  }
  ledStates[4]=analogRead(leds[4]);
  Serial.print(ledStates[4]);
  Serial.print(" ");
  
  putThreshold();
  //botPosition = ledAverage / ledSum;
  botPosition=((ledStates[1]-ledStates[3])*1000)/(ledStates[1]+ledStates[3]);
  Serial.print(botPosition);
  Serial.print("   ");
  
  Serial.print(c);
  Serial.println(" ");
  for(int i=0;i<numLeds;i++){
    Serial.print(TledStates[i]);
    Serial.print(" ");
    }
    
    Serial.println(" ");
   

}

void straight(){
      analogWrite(m11,0);
      analogWrite(m12,140);
      analogWrite(m21,0);
      analogWrite(m22,151);
}

void stopall(){
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,0);
      delay(1000);
}

void Pstraight(){
 pidCalc();
 
 errorAdj();
 motorRun();
}
void specialright()
  {   Serial.print("special right");
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,0);
      delay(100);
      analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);
         delay(350);   
      analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,0);
      delay(200);
      read_leds();
  if(TledStates[center])
  {
    while(TledStates[center])
    { analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,maxspeedright-40);
      analogWrite(m22,0);  
      read_leds();
    }
    while(!TledStates[center])
    { analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,maxspeedright-40);
      analogWrite(m22,0);     
      read_leds();  
    }
  }
  else if(!TledStates[leftFar]&&!TledStates[leftNear])
   {  while(!TledStates[center])
      { analogWrite(m11,0);
        analogWrite(m12,maxspeedleft-40);
        analogWrite(m21,maxspeedright-40);
        analogWrite(m22,0);
        read_leds();  
      }
      while(TledStates[center])
      { analogWrite(m11,0);
        analogWrite(m12,maxspeedleft-40);
        analogWrite(m21,maxspeedright-40);
        analogWrite(m22,0);  
        read_leds();
      }
      while(!TledStates[center])
      { analogWrite(m11,0);
        analogWrite(m12,maxspeedleft-40);
        analogWrite(m21,maxspeedright-40);
        analogWrite(m22,0);
        read_leds();  
      }
   }
   else
   {  while(!TledStates[center])
      { analogWrite(m11,0);
        analogWrite(m12,maxspeedleft-40);
        analogWrite(m21,maxspeedright-40);
        analogWrite(m22,0);
        read_leds();  
      }
   } 
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(300);
           Pstraight();
  }
  
 
  void right()
  {        Serial.print("right");       
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);    
       // while(ledStates[center])
        {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);    
           delay(150);
           read_leds();
        }
        ////rfid();
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0); 
           delay(100);
            
        while(!TledStates[center])
         {  analogWrite(m11,0);
            analogWrite(m12,maxspeedleft-40);
            analogWrite(m21,maxspeedright-40);
            analogWrite(m22,0);
           
            read_leds();    
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);
           Pstraight();
  }
  void right1()
  {        Serial.print("right");       
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);    
        while(TledStates[center])
        {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);    
           delay(300);
           read_leds();
        }
        
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0); 
           delay(100);
            
        while(!TledStates[center])
         {  analogWrite(m11,0);
            analogWrite(m12,maxspeedleft-40);
            analogWrite(m21,maxspeedright-40);
            analogWrite(m22,0);
           
            read_leds();    
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);
           Pstraight();
  }
  void left()
  {      read_leds();
          Serial.print("leftturn");
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);    
           delay(100);
           ////rfid();
        {  analogWrite(m11,0);
           analogWrite(m12,maxspeedleft-40);
           analogWrite(m21,0);
           analogWrite(m22,maxspeedright-40);   
           delay(200);
           read_leds(); 
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0); 
           delay(100);
        while(!TledStates[center])
         {  analogWrite(m11,maxspeedleft-40);
            analogWrite(m12,0);
            analogWrite(m21,0);
            analogWrite(m22,maxspeedright-40); 
            read_leds();   
        }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);
          Pstraight();
  }
  void specialleft()
  {   Serial.print("special left"); 
       analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,0);
      delay(100);
      analogWrite(m11,0);
      analogWrite(m12,maxspeedleft-40);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);
      delay(150);  
       analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,0);
      delay(100);
      read_leds();
  if(TledStates[center])
  {
    while(TledStates[center])
    { analogWrite(m11,maxspeedleft-40);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);  
      read_leds();
    }
    while(!TledStates[center])
    { analogWrite(m11,maxspeedleft-40);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,maxspeedright-40);     
      read_leds();  
    }
  }
  else if(!TledStates[rightFar]&& !TledStates[rightNear])
   {  while(!TledStates[center])
      { analogWrite(m11,maxspeedleft-40);
        analogWrite(m12,0);
        analogWrite(m21,0);
        analogWrite(m22,maxspeedright-40);
        read_leds();  
      }
      while(TledStates[center])
      { analogWrite(m11,maxspeedleft-40);
        analogWrite(m12,0);
        analogWrite(m21,0);
        analogWrite(m22,maxspeedright-40);  
        read_leds();
      }
      while(!TledStates[center])
      {analogWrite(m11,maxspeedleft-40);
        analogWrite(m12,0);
        analogWrite(m21,0);
        analogWrite(m22,maxspeedright-40);  
        read_leds(); 
      }
   }
   else
    {  while(!TledStates[center])
      {analogWrite(m11,maxspeedleft-40);
        analogWrite(m12,0);
        analogWrite(m21,0);
        analogWrite(m22,maxspeedright-40);  
        read_leds(); 
      }
   }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);
           delay(100);
       Pstraight();
  }


void setup() {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT);
  pinMode(A0, INPUT);
 // analogWrite(A2,255);
 // analogWrite(A3,255);
   SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                          // Init MFRC522 card
  mfrc522.PCD_SetRegisterBitMask(mfrc522.RFCfgReg, (0x07<<4));
 lcd.begin(16, 2);
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("a");
}
void change()
  {   analogWrite(m11,0);
      analogWrite(m12,0);
      analogWrite(m21,0);
      analogWrite(m22,40);
       delay(10);
  }
void loop() { 
 // rfid();
  read_leds();
  Pstraight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(botPosition);
  
  // straight();
 
 if(c==25) // iske baad path planning
       {   while(TledStates[center])
          {analogWrite(m11,0);
           analogWrite(m12,50);
           analogWrite(m21,0);
           analogWrite(m22,55);
          }
           analogWrite(m11,0);
           analogWrite(m12,0);
           analogWrite(m21,0);
           analogWrite(m22,0);  
           delay(1000);
                    
   int stopValue;
   int directional;
   int botp,botq;
   int p,q;   
  obvious ();
 
 while (nodesSolved != 5*5){
       copyArray(result,previousResult);
      for (int i = 0; i < 5; i++)
  {
    
    for (int j = 0; j < 5; j++)
      decipherPoint (i, j);
      
     
  } 
     if(noProgress(previousResult,result)){
       assumption();
       
     }
  }

       
 pathPlanning(); 
//Display safe points on LCD as (j,i)
lcd.clear();
lcd.setCursor(0,0);
for(int x=0; x<5; x++)
{  for(y=0; y<5; y++)
   { if(result[x][y]==-1)
     {  lcd.print("(");
        lcd.print(y);
        lcd.print(",");
        lcd.print(x);
        lcd.print(")");
        delay(1000);
        lcd.clear();
     }
   }
}
for(stopValue=0;queue[0][stopValue]!=0||queue[1][stopValue]!=0;stopValue++){}
        stopValue-=2;
       if(queue[0][stopValue]==5-1){
       right1();
       botp=5-1;
       botq=5-1;  
       directional=2;        
       }
       else{
       left1();
             botp=5-1;
       botq=5-1;
       directional=3; 
             } 
     
       while(botq!=0||botp!=0){
       p=queue[0][stopValue];
       q=queue[1][stopValue];  
     if(directional==1){
       if(q==botq&&botp<p){
    nodeCount(&botp,&botq,p,q,&stopValue);
          read_leds();     
          Pstraight();
          
       }
       else if(p==botp&&botq<q){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botp==5-1){
    left1();
    decreaseDirection(&directional);  
    }     
    else{
    specialleft();
    decreaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }
       else if(p==botp&&botq>q){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botp==5-1){
    right1();
    increaseDirection(&directional);   
    }     
    else{
    specialright();
    increaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }         
     
     }
      else if(directional==2){
       if(p==botp&&botq>q){
    nodeCount(&botp,&botq,p,q,&stopValue);
          read_leds();     
          Pstraight();
       }
       else if(q==botq&&botp<p){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botq==0){
    left1();
    decreaseDirection(&directional);  
    }     
    else{
    specialleft();
    decreaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }
       else if(q==botq&&botp>p){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botq==0){
    right1();
    increaseDirection(&directional);  
    }     
    else{
    specialright();
    increaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      }        
    }      
       else if(directional==3){
        if(q==botq&&botp>p){
    nodeCount(&botp,&botq,p,q,&stopValue);
          read_leds();     
          Pstraight();
       }
       else if(p==botp&&botq>q){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botp==0){
    left1();
    decreaseDirection(&directional);  
    }     
    else{
    specialleft();
    decreaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }
       else if(p==botp&&botq<q){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botp==0){
    right1();
    increaseDirection(&directional);  
    }     
    else{
    specialright();
    increaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }         
       
       }   
       else if(directional==4){
          if(p==botp&&botq<q){
    nodeCount(&botp,&botq,p,q,&stopValue);
          read_leds();     
          Pstraight();
       }
       else if(q==botq&&botp>p){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botq==5-1){
    left1();
    decreaseDirection(&directional);  
    }     
    else{
    specialleft();
    decreaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      
       }
       else if(q==botq&&botp<p){
    nodeCount(&botp,&botq,p,q,&stopValue);
    if(botq==5-1){
    right1();
    increaseDirection(&directional);  
    }     
    else{
    specialright();
    increaseDirection(&directional);  
    }     read_leds();
          Pstraight();
      }        
       
       
       
       }   
       
       
       
       }
       }
   if(TledStates[leftFar]&& TledStates[center] &&((c>8 && c<13)|| (c>18&&c<25) || c==0))
   { Serial.print("leftFar"); 
     ++c;
           
     if(c==10 || c==20 || c==21)
       left(); 
        
                   
       
      else if(c==11 || c==1)
       specialleft();
      else
       {  
           while(TledStates[leftFar]|| TledStates[rightFar])
         { Pstraight();
           read_leds();
         }  
       }
   
   }  
 if(TledStates[rightFar]&& TledStates[center]&& ((c>0 &&c<=8) || (c>=12 && c<=18)))
    {  Serial.print("detected rightfar");
      ++c;
           
          
      if(c==5 || c==15 )
       right();
      else if(c==6 || c==16)
       specialright();
                  
      else
      {   
        while(TledStates[leftFar]|| TledStates[rightFar])
         {
             Pstraight();
             read_leds();
         }  
      }
    }
    else if(!TledStates[leftFar]&& !TledStates[rightFar])//aage chalo
     {  Pstraight();
        read_leds();
    }

//   if(TledStates[leftFar] && ((c>=5)||c==0))
//   { Serial.print("detected leftFar");
//    ++c;
//           analogWrite(m11,0);
//           analogWrite(m12,0);
//           analogWrite(m21,0);
//           analogWrite(m22,0);
//           delay(200);    
//       
//           ////rfid();
//           if(c<=6)
//           { analogWrite(m11,0);
//           analogWrite(m12,0);
//           analogWrite(m21,0);
//           analogWrite(m22,0);
//           delay(1500);
//           } 
//           if(c==1) 
//           specialleft();
//       if(c==6||c==7)
//       left(); 
//          if(c==9)
//         {   stopall();
         
//          int stopValue;
//   int directional;
//   int botp,botq;
//   int p,q;   
//  obvious ();
// 
// while (nodesSolved != 5*5){
//       copyArray(result,previousResult);
//      for (int i = 0; i < 5; i++)
//  {
//    
//    for (int j = 0; j < 5; j++)
//      decipherPoint (i, j);
//      
//     
//  } 
//     if(noProgress(previousResult,result)){
//       assumption();
//       
//     }
//  }
// pathPlanning(); 
////Display safe points on LCD as (j,i)
//lcd.clear();
//lcd.setCursor(0,0);
//for(int x=0; x<5; x++)
//{  for(y=0; y<5; y++)
//   { if(result[x][y]==-1)
//     {  lcd.print("(");
//        lcd.print(y);
//        lcd.print(",");
//        lcd.print(x);
//        lcd.print(")");
//        delay(2000);
//     }
//   }
//}
//for(stopValue=0;queue[0][stopValue]!=0&&queue[1][stopValue]!=0;stopValue++){}
//        stopValue-=2;
//       if(queue[0][stopValue]==5-1){
//       right1();
//       botp=5-1;
//       botq=5-1;  
//       directional=2;        
//       }
//       else{
//       left1();
//             botp=5-1;
//       botq=5-1;
//       directional=3; 
//             } 
//     
//       while(botq!=0||botp!=0){
//       p=queue[0][stopValue];
//       q=queue[1][stopValue];  
//     if(directional==1){
//       if(q==botq&&botp<p){
//    nodeCount(&botp,&botq,p,q,&stopValue);     
//          Pstraight();
//       }
//       else if(p==botp&&botq<q){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botp==5-1){
//    left1();
//    decreaseDirection(&directional);  
//    }     
//    else{
//    specialleft();
//    decreaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }
//       else if(p==botp&&botq>q){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botp==5-1){
//    right1();
//    increaseDirection(&directional);  
//    }     
//    else{
//    specialright();
//    increaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }         
//     
//     }
//      else if(directional==2){
//       if(p==botp&&botq>q){
//    nodeCount(&botp,&botq,p,q,&stopValue);     
//          Pstraight();
//       }
//       else if(q==botq&&botp<p){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botq==0){
//    left1();
//    decreaseDirection(&directional);  
//    }     
//    else{
//    specialleft();
//    decreaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }
//       else if(q==botq&&botp>p){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botq==0){
//    right1();
//    increaseDirection(&directional);  
//    }     
//    else{
//    specialright();
//    increaseDirection(&directional);  
//    }     
//          Pstraight();
//      }        
//    }      
//       else if(directional==3){
//        if(q==botq&&botp>p){
//    nodeCount(&botp,&botq,p,q,&stopValue);     
//          Pstraight();
//       }
//       else if(p==botp&&botq>q){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botp==0){
//    left1();
//    decreaseDirection(&directional);  
//    }     
//    else{
//    specialleft();
//    decreaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }
//       else if(p==botp&&botq<q){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botp==0){
//    right1();
//    increaseDirection(&directional);  
//    }     
//    else{
//    specialright();
//    increaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }         
//       
//       }   
//       else if(directional==4){
//          if(p==botp&&botq<q){
//    nodeCount(&botp,&botq,p,q,&stopValue);     
//          Pstraight();
//       }
//       else if(q==botq&&botp>p){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botq==5-1){
//    left1();
//    decreaseDirection(&directional);  
//    }     
//    else{
//    specialleft();
//    decreaseDirection(&directional);  
//    }     
//          Pstraight();
//      
//       }
//       else if(q==botq&&botp<p){
//    nodeCount(&botp,&botq,p,q,&stopValue);
//    if(botq==5-1){
//    right1();
//    increaseDirection(&directional);  
//    }     
//    else{
//    specialright();
//    increaseDirection(&directional);  
//    }     
//          Pstraight();
//      }        
//       
//       
//       
//       }   
//       
//       
//       
//       }
//                 
//          }    
//      else
//       {  
//          while(TledStates[leftFar]|| TledStates[rightFar])
//         { Pstraight();
//           read_leds();
//         }  
//       }
//   }
//   
// if(TledStates[rightFar]&& (c<5&&c!=0))
//    { 
//      Serial.print("detected rightfar");
//      ++c;
//         analogWrite(m11,0);
//           analogWrite(m12,0);
//           analogWrite(m21,0);
//           analogWrite(m22,0);
//           delay(200);    
//       // while(ledStates[center])
///*        {  analogWrite(m11,0);
//           analogWrite(m12,maxspeedleft-40);
//           analogWrite(m21,0);
//           analogWrite(m22,maxspeedright-40);    
//           delay(220);
//           read_leds();
//        }
//           analogWrite(m11,0);
//           analogWrite(m12,0);
//           analogWrite(m21,0);
//           analogWrite(m22,0); 
//           delay(200);
//         */
//           ////rfid();
//           if(c<=6)
//           { analogWrite(m11,0);
//           analogWrite(m12,0);
//           analogWrite(m21,0);
//           analogWrite(m22,0); 
//           delay(1500);
//           }
//      if(c==3)
//       right();
//      else if(c==4)
//       specialright();
//    
//      else
//      {    
//         while(TledStates[leftFar]|| TledStates[rightFar])
//         { Pstraight();
//           read_leds();
//         }  
//      }
//    }
//    else if(!TledStates[leftFar]&& !TledStates[rightFar])//aage chalo
//     {  Pstraight();
//        read_leds();
//    }
 
} 
