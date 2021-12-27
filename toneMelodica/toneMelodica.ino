#include "pitches.h"
#define REFRESH_TIME 10 // how often check for inputs

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

int lastNote, newNote,lastMultiplier,newMultiplier;


//we will use the 12 pins between 22-33  on registers PORTA and PORTC
//DO NOT USE PORTD register. They are used for some internal stuff and cant be set to pullup high


// function to find the rightmost NOT set bit (0) in a byte
int RightmostBit(byte n)
{
  for (int i = 0; i < 8; i++){
    if(((n >> i) & 1)  ==  0){
      return i;
    }
  }
  return -1; // if no set bits are found
}

//same as above but in other directon: find the leftmost 0 bit in the byte
int LeftmostBit(byte n)
{
  //Only check 4 first bits as the rest are unsused in this application  i = 7,6,5,4
  for (int i = 7; i > 3; i--){
    if(((n >> i) & 1)  ==  0){
      return 7-i; 
      // 7->0
      // 6->1...
    }
  }
  return -1; // if no set bits are found
}



// function to read the ports and return the index of the key pressed -1,0,1...,11
int readPorts(){
  //byte pinsA; // 22-29 linear order
  //byte pinsC; // 30-37 reversed order 
  
  int ind = RightmostBit(PINA);
  if(ind != -1){
    return ind;
  }else{
    ind = LeftmostBit(PINC);
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
  if(DEBUG){
    long int t1 = micros();
   lastNote = readPorts();
   long int t2 = micros();
   Serial.print("Reading ports takes: "); Serial.print(t2-t1); Serial.println(" microseconds"); 
  }
}



void loop() {
  newNote = readPorts();
  newMultiplier = ((PINC & 1) + 1);
  //Octave pin 37
  if(newNote != lastNote || lastMultiplier != newMultiplier){
    lastNote =  newNote;
    lastMultiplier = newMultiplier;
    if(lastNote == -1){
      noTone(8); // stop playing if no keys are pressed
    }else{
      tone(8, lastMultiplier * octave[lastNote]); //play indefinitely while on same note
    }
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
   }
  delay(REFRESH_TIME);
}


/*
byte readPorts(){
  //byte pinsA; // 22-29
  //byte pinsC; // 30-37
  
  byte ind = RightmostBit(PINA);
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

byte flipByte(byte c){
  char r=0;
  for(byte i = 0; i < 8; i++){
    r <<= 1;
    r |= c & 1;
    c >>= 1;
  }
  return r;
}
