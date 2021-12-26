#include "pitches.h"
#define REFRESH_TIME 1000 // how often check for inputs

#define DEBUG false

int Lick[] = {
  NOTE_G4,
  NOTE_A4,
  NOTE_AS4,
  NOTE_C5,
  NOTE_A4,
  NOTE_F4,
  NOTE_G4
  };
  
int LickDurations[] = {
   4, 4, 4, 4,2,4,4 
};

int octave[] = {
    NOTE_C4,
    NOTE_CS4,
    NOTE_D4,
    NOTE_DS4,
    NOTE_E4,
    NOTE_F4,
    NOTE_FS4,
    NOTE_G4,
    NOTE_GS4,
    NOTE_A4,
    NOTE_AS4,
    NOTE_B4
};

char lastNote;
//http://harperjiangnew.blogspot.com/2013/05/arduino-port-manipulation-on-mega-2560.html
//we will use the 12 pins between 22-33  on registers PORTA and PORTC
//DO NOT USE PORTD register. They are used for some internal stuff and cant be set to pullup high


// function to find the rightmost NOT set bit (0) in a char
char RightmostBit(char n)
{
  for (char i = 0; i < 8; i++){
    if(((n >> i) & 1)  ==  0){
      return i;
    }
  }
  return -1; // if no set bits are found
}
// function to read the ports and return the index of the key pressed -1,0,1...,11
char readPorts(){
  //char pinsA; // 22-29
  //char pinsC; // 30-37
  
  char ind = RightmostBit(PINA);
  if(ind != -1){
    return ind;
  }else{
    ind = RightmostBit(PINC);
    if(ind != -1){
      return ind + 8;
    }else{
      return -1;
    }
  }
}


void setup() {
  for(int i = 22; i <38; i++){
    pinMode(i, INPUT_PULLUP);
  }
  Serial.begin(9600);
  //play the lick on setup
  for (int thisNote = 0; thisNote < 7; thisNote++) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/LickDurations[thisNote];
      tone(8, Lick[thisNote], noteDuration);
      //pause for the note's duration plus 30 ms:
      delay(noteDuration +30);
   }
}

void loop() {
  lastNote = readPorts();
  if(lastNote != -1){
    tone(8, octave[lastNote] ,REFRESH_TIME);
  }
  if(DEBUG){
      Serial.print("Playing note with index: "); 
      Serial.print(lastNote, DEC); 
      Serial.println(); 

      Serial.print("PINA: "); 
      Serial.print(PINA, BIN); 
      Serial.println(); 

      Serial.print("PINC: "); 
      Serial.print(PINC, BIN); 
      Serial.println(); 

      Serial.print("PIND: "); 
      Serial.print(PIND, BIN); 
      Serial.println(); 
   }
  delay(REFRESH_TIME);
}


/*
char readPorts(){
  //char pinsA; // 22-29
  //char pinsC; // 30-37
  
  char ind = RightmostBit(PINA);
  if(ind != -1){
    return ind/2;
  }else{
    ind = RightmostBit(PINC);
    if(ind != -1){
      return ind/2 + 4;
    }else{
      ind = RightmostBit(PIND);
      if(ind != -1){
        return ind/2 + 8;
      }else{
        return -1;
      }
    }
  }
}
*/
