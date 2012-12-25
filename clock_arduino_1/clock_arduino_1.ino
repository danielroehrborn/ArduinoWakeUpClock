#include <Time.h>
#include <TimeAlarms.h>
#include <SoftwareSerial.h>

#define SERIALON
//#define BLUETOOTHON

#define SCREEN_RX_PIN  3
#define SCREEN_TX_PIN  2
#define BLUE_RX_PIN  0
#define BLUE_TX_PIN  1

char out[5]="0000";
char oldhour = 0, oldminute = 0, oldsecond = 0, oldday = 0, oldmonth = 0, oldalarmh = 0, oldalarmm = 0, oldalarmon = 0;
int sendSecondly = 0;
int oldyear = 0;
char in[4];
char charIn;
char blueIn;
char blueString[10];
char alarmOn = 0;
char temp;
int i=0;
time_t aTime;

#ifdef SERIALON
SoftwareSerial touchSerial(SCREEN_RX_PIN, SCREEN_TX_PIN);
#endif SERIALON
#ifdef BLUETOOTHON
SoftwareSerial blueToothSerial(BLUE_RX_PIN, BLUE_TX_PIN);
#endif BLUETOOTHON

void setup()
{
  delay(6000); //display ready
  pinMode(BLUE_RX_PIN,INPUT);
  pinMode(BLUE_TX_PIN,OUTPUT);

#ifdef BLUETOOTHON
  setupBlueToothConnection();
#endif BLUETOOTHON

#ifdef SERIALON
  Serial.begin(9600);
  touchSerial.begin(9600);
#endif
  setTime(23,59,50,31,12,12);

  Alarm.alarmRepeat(23,59,00, Alarm1);//Alarm.alarmRepeat(23,59,55, Alarm1);//AlarmID==0
  Alarm.disable(0);
  //Alarm.alarmRepeat(00,00,01, Alarm2);

  //Alarm.alarmRepeat(dowSunday,1,1,0, Alarm2);
  //alarmid = Alarm.timerRepeat(3, Repeats);
  //->in TimeAlarms.cpp: #include <Arduino.h> //ehemals WProgram.h
  //->in TimeAlarms.h ServiceAlarms als public
  //->in time.h bestehendes ersetzen durch:
  //#define minutesToTime_t(M) ( (M) * SECS_PER_MIN)
  //#define hoursToTime_t(H) ( H * SECS_PER_HOUR)
  //#define daysToTime_t(D) ( (D) * SECS_PER_DAY) // fixed on Jul 22 2011
  //#define weeksToTime_t(W) ( (W) * SECS_PER_WEEK)
  //->in TimeAlarms.cpp:Unter isAllocated das einfügen: Alarm[ID].nextTrigger = 1;
  //http://code.google.com/p/clockthree/source/browse/trunk/libraries/TimeAlarms/TimeAlarms.cpp?spec=svn184&r=184
  //http://arduino-time.googlecode.com/svn/trunk/Time.h
  //Alarm.enable(0);
  //Alarm.disable(0);
  //Alarm.free(0);
  //Alarm.write(0,time_t==unsigned long);
  //Alarm.read(0)returns time_t, minute(time_t) returns int
  //Serial.println(hour(Alarm.read(0)));
  //Serial.println(minute(Alarm.read(0)));
  //Serial.println(second(Alarm.read(0)));
  //Alarm.write(0, hoursToTime_t(22) + minutesToTime_t(58) + 50 );
  //Serial.println(hour(Alarm.read(0)));
  //Serial.println(minute(Alarm.read(0)));
  //Serial.println(second(Alarm.read(0)));
}
void Repeats(){
  //Serial.println("Repeats\n");
}
void Alarm1(){
  Serial.println("Alarm1\n");
  //sprintf(out, "a++a");
  alarmOn = 1;
}
void Alarm2(){
  Serial.println("Alarm2\n");
  //sprintf(out, "a--a");
  alarmOn = 0;
}

void loop()
{
  //sprintf(timedate, "%02d%02d%02d%02d%02d%04d", hour(), minute(), second(), day(), month(), year());
  //Serial.println(timedate);
  //Alarm.delay(1); //serviceAlarms()
  //TimeAlarms.h ServiceAlarms als public
  Alarm.serviceAlarms();

  if(out[0] == '0')
  {
    if(oldday != day()){
      oldday = day();
      sprintf(out, "d%02dd", oldday);
    }
    else
      if(oldmonth != month()){
        oldmonth = month();
        sprintf(out, "o%02do", oldmonth);
      }
      else
        if(oldyear != year()){
          oldyear = year();
          sprintf(out, "y%02dy", oldyear%100);
        }
        else
          if(oldalarmh != hour(Alarm.read(0))){
            oldalarmh = hour(Alarm.read(0));
            sprintf(out, "q%02dq", hour(Alarm.read(0)));
          }
          else
            if(oldalarmm != minute(Alarm.read(0))){
              oldalarmm = minute(Alarm.read(0));
              sprintf(out, "z%02dz", minute(Alarm.read(0)));
            }
            else
            if(oldalarmon != alarmOn){
              oldalarmon = alarmOn;
              if(alarmOn)
                sprintf(out, "a++a");
              else sprintf(out, "a--a");
            }
            else
              if(oldhour != hour()){
                oldhour = hour();
                sprintf(out, "h%02dh", oldhour);
              }
              else
                if(oldminute != minute()){
                  oldminute = minute();
                  sprintf(out, "m%02dm", oldminute);
                }
                else
                  if(oldsecond != second()){
                    oldsecond = second();
                    sprintf(out, "s%02ds", oldsecond);
                    //alle aktualisieren, falls es irgendwann mal
                    //übertragungsfehler gegeben haben sollte:
                    if(oldsecond == 10)
                      oldminute = -1;
                    else if(oldsecond == 20)
                      oldhour = -1;
                    else if(oldsecond == 30)
                      oldday = -1;
                    else if(oldsecond == 40)
                      oldmonth = -1;
                    else if(oldsecond == 50)
                      oldyear = -1;
                    else if(oldsecond == 5)
                      oldalarmon = -1;
                      /*if(alarmOn)
                        sprintf(out, "a++a");
                      else sprintf(out, "a--a");*/
                    else if(oldsecond == 25)
                      oldalarmh = -1;
                    //sprintf(out, "q%02dq", hour(Alarm.read(0)));
                    else if(oldsecond == 35)
                      oldalarmm = -1;
                    //sprintf(out, "z%02dz", minute(Alarm.read(0)));
                  }
  }

/*
  //benutzereingabe
  if(Serial.available())
  {  
    i = 0;
    delay(10);  
    while(i<4)
      out[i++] = Serial.read();
    //Serial.print("Input: ");
    // Serial.println(out);
    if(out[0] == 'x' && out[3] == 'x')
    {
      temp = ((out[1]-48)<<3) + ((out[1]-48)<<1) + (out[2]-48);
      time_t hTime = AlarmHMS( temp, minute(Alarm.read(0)), 0);
      Alarm.write(0, hTime);
      sprintf(out, "q%02dq", hour(Alarm.read(0)));
      //Alarm.enable(0);
    }
    else
      if(out[0] == 'y' && out[3] == 'y')
      {
        temp = ((out[1]-48)<<3) + ((out[1]-48)<<1) + (out[2]-48);
        time_t mTime = AlarmHMS( hour(Alarm.read(0)), temp, 0);
        Alarm.write(0, mTime);    
        sprintf(out, "z%02dz", minute(Alarm.read(0)));
        //Alarm.enable(0);
      }

  }
*/


#ifdef SERIALON
  if(sendSecondly != millis()>>8){
    //Serial.println(out);
    sendSecondly = millis()>>8;//div 512 = 2mal pro sec
    if(out[0] != '0'){
      Serial.println(out);
      touchSerial.write(out);
      out[0] ='0';
    }
  }
#endif SERIALON


//#ifdef BLUETOOTHON
  if(Serial.available()>0) //blueToothSerial
  {
    delay(10); //warten bis alles da ist
    i = 0;
    while(Serial.available() && i<10) //blueToothSerial
    {
      blueIn = Serial.read(); //blueToothSerial
      blueString[i++] = blueIn;
    }
    Serial.println("blueString");
    Serial.println(blueString);
    /*if(blueIn == 'h'){
      setTime(hour()+1,minute(),second(),day(),month(),year());
    }
    else if(blueIn == 'm'){
      setTime(hour(),minute()+1,second(),day(),month(),year());
    }
    else if(blueIn == 'd'){
      setTime(hour(),minute(),second(),day()+1,month(),year());
    }
    else if(blueIn == 'k'){
      setTime(hour(),minute(),second(),day(),month()+1,year());
    }
    else if(blueIn == 'y'){
      setTime(hour(),minute(),second(),day(),month(),year()+1);
    }
    else */
    if(blueString[0] == 'h' && blueString[3] == 'h') //set Hour "h12h"
    {
      temp = ((blueString[1]-48)<<3) + ((blueString[1]-48)<<1) + (blueString[2]-48);
      setTime(temp,minute(),second(),day(),month(),year());
      //nach zeitumstellung muss alarmzeit aktualisiert werden, sonts kein alarm
      aTime = AlarmHMS( hour(Alarm.read(0)), minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
    }
    else if(blueString[0] == 'm' && blueString[3] == 'm') //set Minute "m12m"
    {
      temp = ((blueString[1]-48)<<3) + ((blueString[1]-48)<<1) + (blueString[2]-48);
      setTime(hour(),temp,second(),day(),month(),year());
      aTime = AlarmHMS( hour(Alarm.read(0)), minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
    }
    else if(blueString[0] == 'd' && blueString[3] == 'd') //set Day "d12d"
    {
      temp = ((blueString[1]-48)<<3) + ((blueString[1]-48)<<1) + (blueString[2]-48);
      setTime(hour(),minute(),second(),temp,month(),year());
      aTime = AlarmHMS( hour(Alarm.read(0)), minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
    }
    else if(blueString[0] == 'o' && blueString[3] == 'o') //set Month "m12m"
    {
      temp = ((blueString[1]-48)<<3) + ((blueString[1]-48)<<1) + (blueString[2]-48);
      setTime(hour(),minute(),second(),day(),temp,year());
      aTime = AlarmHMS( hour(Alarm.read(0)), minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
    }
    else if(blueString[0] == 'y' && blueString[3] == 'y') //set Year "y12y"
    {
      temp = ((blueString[1]-48)<<3) + ((blueString[1]-48)<<1) + (blueString[2]-48);
      setTime(hour(),minute(),second(),day(),month(),2000+temp);
      aTime = AlarmHMS( hour(Alarm.read(0)), minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
    }
    else if(blueString[0] == 'a' && blueString[3] == 'a') //enable/disable Alarm "a++a" "a--a"
    {
      if(blueString[1] == '+' && blueString[2] == '+')
      {
        //alarmOn=1;
        Alarm.enable(0);
        Serial.println("Alarm: On\n");
      }
      else if(blueString[1] == '-' && blueString[2] == '-')
      {
        //alarmOn=0;
        Alarm.disable(0);
        Serial.println("Alarm: Off\n");
      }else if(blueString[1] == 'a' && blueString[2] == 'a')
      {
        alarmOn=alarmOn?0:1;
        Alarm.enable(0);
        Serial.println("Alarm: Test\n");
      }
    }
    else if(blueString[0] == 'a' && blueString[1] == 'h' && blueString[4] == 'a' && blueString[5] == 'h') //set Alarm Hour "ah12ah"
    {
      temp = ((blueString[2]-48)<<3) + ((blueString[2]-48)<<1) + (blueString[3]-48);
      aTime = AlarmHMS( temp, minute(Alarm.read(0)), 0);
      Alarm.write(0, aTime);
      sprintf(out, "q%02dq", hour(Alarm.read(0)));
      Alarm.enable(0);
    }
    else if(blueString[0] == 'a' && blueString[1] == 'm' && blueString[4] == 'a' && blueString[5] == 'm') //set Alarm Minute "an12am"
    {
      temp = ((blueString[2]-48)<<3) + ((blueString[2]-48)<<1) + (blueString[3]-48);
      aTime = AlarmHMS( hour(Alarm.read(0)), temp, 0);
      Alarm.write(0, aTime);    
      sprintf(out, "z%02dz", minute(Alarm.read(0)));
      Alarm.enable(0);
    }
    else if(blueString[0] == 'c') //set Font Color "c1r255"->colornr 1 red set to 255
    {
      //z.B c1r200 -> save "200" as 200 in temp
      temp = ((blueString[3]-48)<<6) + ((blueString[3]-48)<<5) + ((blueString[3]-48)<<2) +
             ((blueString[4]-48)<<3) + ((blueString[4]-48)<<1) + ((blueString[5]-48));  
             
      if(blueString[1] == '1') //ColorNr 1 = default orange Schriftfarbe
      {
        if(blueString[2] == 'r') //rotanteil
          sprintf(out, "1%03d", temp);    
        else if(blueString[2] == 'g') //grünanteil  
          sprintf(out, "2%03d", temp);    
        else if(blueString[2] == 'b') //blauanteil     
          sprintf(out, "3%03d", temp);       
      }
      else if(blueString[1] == '2') //ColorNr 2 = default braune Schriftfarbe
      {
        if(blueString[2] == 'r') //rotanteil
          sprintf(out, "4%03d", temp);    
        else if(blueString[2] == 'g') //grünanteil  
          sprintf(out, "5%03d", temp);    
        else if(blueString[2] == 'b') //blauanteil     
          sprintf(out, "6%03d", temp);   
      }
    }
  }
//#endif BLUETOOTHON

}

#ifdef BLUETOOTHON
void setupBlueToothConnection()
{
  //blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.begin(38400);
  delay(1000);
  sendBlueToothCommand("\r\n+STWMOD=0\r\n");
  sendBlueToothCommand("\r\n+STNA=SeeeduinoBluetooth\r\n");
  sendBlueToothCommand("\r\n+STAUTO=0\r\n");
  sendBlueToothCommand("\r\n+STOAUT=1\r\n");
  sendBlueToothCommand("\r\n +STPIN=0000\r\n");
  delay(2000); // This delay is required.
  sendBlueToothCommand("\r\n+INQ=1\r\n");
  delay(2000); // This delay is required.
}

//Checks if the response "OK" is received
void CheckOK()
{
  char a,b;
  while(1)
  {
    if(blueToothSerial.available())
    {
      a = blueToothSerial.read();

      if('O' == a)
      {
        // Wait for next character K. available() is required in some cases, as K is not immediately available.
        while(blueToothSerial.available())
        {
          b = blueToothSerial.read();
          break;
        }
        if('K' == b)
        {
          break;
        }
      }
    }
  }

  while( (a = blueToothSerial.read()) != -1)
  {
    //Wait until all other response chars are received
  }
}

void sendBlueToothCommand(char command[])
{
  blueToothSerial.print(command);
  CheckOK();
}
#endif BLUETOOTHON




