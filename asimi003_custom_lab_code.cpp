/*        Your Name & E-mail: Angelica Simityan   asimi003@ucr.edu

 *         Discussion Section: 021

 *         Assignment: CUSTOM LAB FINAL DEMO

 *         Exercise Description: [optional - include for your own benefit]

 *

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link:

 https://youtu.be/0bbu1bGP9G0

 */

// Array of Output Pin variables, set to the pins being used

#include "dht.h"
#include <LiquidCrystal.h>
#define dht_apin A0 // Analog Pin sensor is connected to
 
dht DHT;
 
// Array of Output Pin variables, set to the pins being used
const int rs = 4, en = 5, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int trigPin = 3;
const int echoPin = 2;

float duration, distance;

const int lightpin = 12;

const int motorpin = 6;

bool plant = false;






// Reset the Output Buffer. 


typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum SM1_States { SM1_INIT, SM1_S0, SM1_S1, SM1_S2, SM1_S3};
int SM1_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM1_INIT:
         //State Transition
            state = SM1_S1;
        break;
      case SM1_S0:
        if(plant == false){
          state = SM1_S3;
        }
        else if (humidity() < 63)
        {
          state = SM1_S0;
        }
        else if ((63 < humidity())  &&  (humidity()< 68))
        {
          state = SM1_S1;
        }
         else if (humidity() > 68)
        {
          state = SM1_S2;
        }
         //State Transition
        break;
      case SM1_S1:
      if(plant == false){
          state = SM1_S3;
        }
        else if (humidity() < 63)
        {
          state = SM1_S0;
        }
        else if ((63 < humidity())  &&  (humidity()< 68))
        {
          state = SM1_S1;
        }
         else if (humidity() > 68)
        {
          state = SM1_S2;
        }    
        break;
        case SM1_S2:
        if(plant == false){
          state = SM1_S3;
        }
        else if (humidity() < 63)
        {
          state = SM1_S0;
        }
        else if ((63 < humidity())  &&  (humidity()< 68))
        {
          state = SM1_S1;
        }
         else if (humidity() > 68)
        {
          state = SM1_S2;
        }
        
        break;

        case SM1_S3:
        if(plant == false){
          state = SM1_S3;
        }
        else if (humidity() < 63)
        {
          state = SM1_S0;
        }
        else if ((63 < humidity())  &&  (humidity()< 68))
        {
          state = SM1_S1;
        }
         else if (humidity() > 68)
        {
          state = SM1_S2;
        }
        break;
    }
    switch(state){ // State Action
      case SM1_INIT:
          
         //State Action
        break;
      case SM1_S0:
            //State Action
            
            
           digitalWrite(motorpin, LOW);
           LCDdisp(0);
           
           
        break;
       
       case SM1_S1:

         digitalWrite(motorpin, LOW);
         LCDdisp(1);
            
        break;

        case SM1_S2:

        digitalWrite(motorpin, HIGH);
        LCDdisp(2);
           
        break;

        case SM1_S3:

        digitalWrite(motorpin, LOW);
        LCDdisp(3);
        
        break;
    }

    return state;
}

enum SM2_States { SM2_INIT, SM2_S0, SM2_S1};
int SM2_Tick(int state){
    //Read thing
    switch(state){ // State transitions
      case SM2_INIT:
         //State Transition
           state = SM2_S0;
        break;
      case SM2_S0:
            
        //State Transition
        if(ultrasonic() < 25){
          state = SM2_S1;
        }
        break;

        case SM2_S1:
        if(ultrasonic() > 25){
          state = SM2_S0;
        }
        break;
  
    }
    switch(state){ // State Action
      case SM2_INIT:
         //State Action
        break;
      case SM2_S0:
            //State Action
  
            digitalWrite(lightpin, LOW);
            plant = false;
            
          
         //State Action
        break;

        case SM2_S1:

       digitalWrite(lightpin, HIGH);
       plant = true;
        
        break;
     
    }

    return state;
}






void setup() {
    //some set up (Default Arduino Function)
 
     lcd.begin(16,2);
  unsigned char i = 0;
  tasks[i].state = SM1_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;
  tasks[i].state = SM2_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   pinMode(lightpin, OUTPUT);
   pinMode(motorpin, OUTPUT);
  //delay_gcd = 500; // GCD

}


void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
  //delay(delay_gcd); // GCD.
}

float ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.println(distance);
  return distance;
}


float humidity(){
  DHT.read11(dht_apin);

  
  return DHT.humidity;
}


void LCDdisp(int state_disp ){
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Hum: ");
 lcd.setCursor(5,0);
 lcd.print(humidity());
 lcd.setCursor(0,1);
 if(state_disp == 1){
  lcd.print("Regular");
 }
 else if(state_disp == 2)
 {
    lcd.print("Too Wet");
 }
 else if(state_disp == 3)
 {
    lcd.print("Plant Not There");
 }
 else if(state_disp == 0)
 {
    lcd.print("Water Me");
 }
 
}
