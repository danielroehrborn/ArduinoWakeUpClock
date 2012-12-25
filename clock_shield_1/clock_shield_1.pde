#include "HersheyFonts.h"   //needed for dispPutXLargeNumString()
#include <TButton.h>
//COLOR blue = {0,0,255};
//COLOR orange =  {255, 170, 50};
//COLOR green =  {170, 255, 0};
//COLOR black = {0,0,0};

#define DEBUGON

COLOR backCol = {
  0,0,0}; //background black
COLOR fontCol =  {
  255, 170, 50}; //orange
COLOR fontCol2 =  {
  120, 80, 0}; //dark orange
COLOR alCol1 = {
  200,0,0}; //alarmColor1 red
COLOR alCol2 = {
  50,0,0}; //alarmColor2 dark red
COLOR frameCol = {
  9,9,9}; //boxframe grey
COLOR debugCol = {
  0,0,255}; //blue

char received[5];
char displaySec[3];
char displayMinute[3];
char displayHour[3];
char displayDay[3];
char displayMonth[3];
char displayYear[5]="2000";
char displayAlarmHour[3];
char displayAlarmMinute[3];
char alarmOn=0;
char alarmDelay=0;

void doAlarm(){
  if(alarmOn==1)
  {
    stroke(alCol1.red,alCol1.green,alCol1.blue);
    fill(alCol1.red,alCol1.green,alCol1.blue);
    alarmOn=2;
  }
  else if(alarmOn==2)
  {
    stroke(alCol2.red,alCol2.green,alCol2.blue);
    fill(alCol2.red,alCol2.green,alCol2.blue);  
    alarmOn=1;
  }
  text(displayAlarmHour, 25, 140, 25);
  text(":", 75, 140, 30);
  text(displayAlarmMinute, 100, 140, 25);
  text("Alarm", 80, 195, 30);

  stroke(backCol.red,backCol.green,backCol.blue);
  rect(15,160,10,10);
  rect(35,160,10,10);
  rect(55,160,10,10);
  rect(75,160,10,10);
  rect(95,160,10,10);
  rect(115,160,10,10);
  rect(135,160,10,10);
  rect(155,160,10,10);
  rect(175,160,10,10);
  rect(195,160,10,10);
  rect(215,160,10,10);
  rect(235,160,10,10);
  rect(255,160,10,10);
  rect(275,160,10,10);
  rect(295,160,10,10);
  rect(15,180,10,10);
  rect(295,180,10,10);
  rect(15,200,10,10);
  rect(295,200,10,10);
  rect(15,220,10,10);
  rect(35,220,10,10);
  rect(55,220,10,10);
  rect(75,220,10,10);
  rect(95,220,10,10);
  rect(115,220,10,10);
  rect(135,220,10,10);
  rect(155,220,10,10);
  rect(175,220,10,10);
  rect(195,220,10,10);
  rect(215,220,10,10);
  rect(235,220,10,10);
  rect(255,220,10,10);
  rect(275,220,10,10);
  rect(295,220,10,10);
}

void setup() 
{  
  Serial.begin(9600);
  background(backCol.red,backCol.green,backCol.blue);
} 

char charIn = 0;
byte i = 0;

int counttoalarmoff=0;

void loop() 
{

  if(Serial.available())
  {  
    delay(10); //gegen Fehler
    i = 0;
    while(Serial.available())
    {
      charIn = Serial.read();
      received[i++] = charIn;
    }  
    makeDisplay();
  }

  if(alarmOn)
  {
    if(alarmDelay++ == 8)
    {
      alarmDelay = 0;
      doAlarm();
    }
  }
  delay(100);
}

void makeDisplay(){

  if(received[0] == 's' && received[3] == 's'){
    displaySec[0]=received[1];
    displaySec[1]=received[2];
    displaySec[2]=0;
    dispPutLargeNumString(displaySec, 260, 70, fontCol, backCol);
  }
  else
    if(received[0] == 'm' && received[3] == 'm'){
      displayMinute[0]=received[1];
      displayMinute[1]=received[2];
      displayMinute[2]=0;
      dispPutXLargeNumString(displayMinute, 130, 10, fontCol, backCol);
    }
    else
      if(received[0] == 'h' && received[3] == 'h'){
        displayHour[0]=received[1];
        displayHour[1]=received[2];
        displayHour[2]=0;
        dispPutXLargeNumString(displayHour, 10, 10, fontCol, backCol);
      }
      else
        if(received[0] == 'd' && received[3] == 'd'){
          stroke(backCol.red, backCol.green, backCol.blue);
          text(displayDay, 120, 110, 15);
          displayDay[0]=received[1];
          displayDay[1]=received[2];
          displayDay[2]=0;
            //stroke(frameCol.red,frameCol.green,frameCol.blue);
          //fill(backCol.red,backCol.green,backCol.blue);
          //rect(111,98,35,22);
          stroke(fontCol.red, fontCol.green, fontCol.blue);//stroke(255,170,50);
          text(displayDay, 120, 110, 15);
        }
        else
          if(received[0] == 'o' && received[3] == 'o'){
            stroke(backCol.red, backCol.green, backCol.blue);
            text(displayMonth, 160, 110, 15);
            displayMonth[0]=received[1];
            displayMonth[1]=received[2];
            displayMonth[2]=0;
            //stroke(frameCol.red,frameCol.green,frameCol.blue);
            //fill(backCol.red,backCol.green,backCol.blue);
            //rect(151,98,35,22);
            stroke(fontCol.red, fontCol.green, fontCol.blue);
            text(displayMonth, 160, 110, 15);
          }
          else
            if(received[0] == 'y' && received[3] == 'y'){
              stroke(backCol.red, backCol.green, backCol.blue);
              text(displayYear, 200, 110, 15);
              displayYear[2]=received[1];
              displayYear[3]=received[2];
              displayYear[4]=0;
              //stroke(frameCol.red,frameCol.green,frameCol.blue);
              //fill(backCol.red,backCol.green,backCol.blue);
              //rect(190,98,73,22);
              stroke(fontCol.red, fontCol.green, fontCol.blue);
              text(displayYear, 200, 110, 15);
            }
            else
              if(received[0] == 'a' && received[1] == '+' && received[2] == '+' && received[3] == 'a'){
                alarmOn = 1;
                //#ifdef DEBUGON
                //stroke(alCol1.red,alCol1.green,alCol1.blue);
                //text("A", random(300), random(200), 5);
                //#endif DEBUGON
              }
              else
                if(received[0] == 'a' && received[1] == '-' && received[2] == '-' && received[3] == 'a'){
                  alarmOn = 0;
                  fill(backCol.red,backCol.green,backCol.blue);
                  stroke(backCol.red,backCol.green,backCol.blue);
                  rect(15,160,305,230);
                  //#ifdef DEBUGON
                  //stroke(alCol1.red,alCol1.green,alCol1.blue);
                  //text("a", random(300), random(200), 5);
                  //#endif DEBUGON
                }
                else
                  if(received[0] == 'q' && received[3] == 'q'){
                    stroke(backCol.red, backCol.green, backCol.blue);
                    text(displayAlarmHour, 25, 140, 25);
                    text(":", 75, 140, 30);
                    displayAlarmHour[0]=received[1];
                    displayAlarmHour[1]=received[2];
                    displayAlarmHour[2]=0;
                    //stroke(frameCol.red,frameCol.green,frameCol.blue);
                    //fill(backCol.red,backCol.green,backCol.blue);
                    //rect(10,122,70,33);
                    stroke(fontCol2.red,fontCol2.green,fontCol2.blue);
                    text(displayAlarmHour, 25, 140, 25);
                    text(":", 75, 140, 30);
                  }
                  else
                    if(received[0] == 'z' && received[3] == 'z'){
                      stroke(backCol.red, backCol.green, backCol.blue);
                      text(displayAlarmMinute, 100, 140, 25);
                      displayAlarmMinute[0]=received[1];
                      displayAlarmMinute[1]=received[2];
                      displayAlarmMinute[2]=0;
                      //stroke(frameCol.red,frameCol.green,frameCol.blue);
                      //fill(backCol.red,backCol.green,backCol.blue);
                      //rect(88,122,55,33);
                      stroke(fontCol2.red,fontCol2.green,fontCol2.blue);
                      text(displayAlarmMinute, 100, 140, 25);
                    }
                    else
                      if(received[0] == '1'){ //fontColor.red  oder //atoi(&received[1]);
                        fontCol.red = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                          ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                      }
                      else
                        if(received[0] == '2'){ //fontColor.green
                          fontCol.green = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                            ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                        }
                        else
                          if(received[0] == '3'){ //fontColor.blue
                            fontCol.blue = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                              ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                          }
                          else
                            if(received[0] == '4'){ //fontColor2.red  oder //atoi(&received[1]);
                              fontCol2.red = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                                ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                            }
                            else
                              if(received[0] == '5'){ //fontColor2.green
                                fontCol2.green = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                                  ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                              }
                              else
                                if(received[0] == '6'){ //fontColor2.blue
                                  fontCol2.blue = ((received[1]-48)<<6) + ((received[1]-48)<<5) + ((received[1]-48)<<2) +
                                    ((received[2]-48)<<3) + ((received[2]-48)<<1) + ((received[3]-48));
                                }
#ifdef DEBUGON
                                else 
                              {
                                stroke(debugCol.red,debugCol.green,debugCol.blue);
                                text(received, random(300), random(200), 5);
                                //text("x", random(300), random(200), 5);
                              }
#endif DEBUGON

}

