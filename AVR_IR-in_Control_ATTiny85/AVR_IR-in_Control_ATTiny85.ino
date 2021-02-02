#include <avr/sleep.h>

//IR control commands
#define ADDRESS 0xA5 //address of the Pioneer amplifier A-20-S
#define POWER_TOGGLE 0x38 //command for power toggle on/off
#define POWER_ON 0x58 //command for power on
#define INPUT_NETWORK_1 0x3A  //first command for input selection network 
#define INPUT_NETWORK_2 0xE3  //second command for input selection network

//CONTROL IN Signal logic and timing
#define NORMAL LOW  //turn LOW for usage of inverting transistor
#define SIG HIGH  //turn HIGH for usage of inverting transistor
#define SHORT (562-40)  //duration of "pulse burst" / short delay in µs;theoretical value is 562µs
#define LONG (675-154)  //duration of long delay in µs minus 1ms which is covered by delay(1); theoretical value is 675µs
#define INTERVAL 700  //interval in µs between repition of commands minus 26ms which are covered by delay(26)

//Pins
#define OUT_PIN 2 //hardware pin 7; analog pin 1
#define IN_PIN 1  //hardware pin 6

//Interrupts
#define USED_INTERRUPT 1  //used external interrupt

//Variables
bool firstLoop = true;

void setup() {
  //pin change interrupt setup
  GIMSK |= (1<<PCIE);  // turns on pin change interrupts (bit 5)
  PCMSK |= (1<<PCINT1); // turn on interrupts on pin PB1
  sei();  //set enable interrupt
  pinMode(IN_PIN, INPUT);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, NORMAL);
}

void loop() {
  
  if(digitalRead(IN_PIN)){
    //turn on the amplifier
    for(int c=0; c<3; c++){
      sendCommand(POWER_ON);
      delay(26);
      delayMicroseconds(INTERVAL);
    }
    //wait 5 seconds for it to start    
    delay(5000);

    for(int n=0; n<5; n++){   //wiederholt, da unzuverlässig
      //select NETWORK as input
      for(int d=0; d<2; d++){
        sendCommand(INPUT_NETWORK_1);
        delay(26);
        delayMicroseconds(INTERVAL);
        sendCommand(INPUT_NETWORK_2);
        delay(26);
        delayMicroseconds(INTERVAL);
      }
      delay(1000);
    }
  }
  else{
    if(!firstLoop){
      //turn off the amplifier using on/off
      for(int c=0; c<3; c++){
        sendCommand(POWER_TOGGLE);
        delay(26);
        delayMicroseconds(INTERVAL);
      }
    }
  }
  firstLoop = false;
  //change=false;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //set sleep mode
  sleep_enable();        //ready to sleep
  sleep_cpu();          // sleep                
  sleep_disable ();      // precaution  
  delay(100);
  
}


void sendCommand(int com){
  //initial high
  digitalWrite(OUT_PIN, SIG);
  delay(8);
  delayMicroseconds(520);
  //initial low
  digitalWrite(OUT_PIN, NORMAL);
  delay(4);
  delayMicroseconds(180);
  //Address
  sendOneByte(ADDRESS);
  sendOneByte(255-ADDRESS);
  //Command
  sendOneByte(com);
  sendOneByte(255-com);
  //ending
  digitalWrite(OUT_PIN, SIG);
  delayMicroseconds(SHORT);
  digitalWrite(OUT_PIN, NORMAL);
}

void sendOneByte(int data){
  for(int b=7; b>=0; b--){
    //allways high
    digitalWrite(OUT_PIN, SIG);
    delayMicroseconds(SHORT);
    digitalWrite(OUT_PIN, NORMAL);
    //bit defines duration of low
    if(bitRead(data, b)==1){
      delay(1);
      delayMicroseconds(LONG);
    }
    else{
      delayMicroseconds(SHORT);
    }
  }
}

ISR(PCINT0_vect){
  //do nothing
}
