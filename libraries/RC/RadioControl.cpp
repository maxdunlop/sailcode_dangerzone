#include <RCstructs.h>

#define STD_TIMEOUT 20000 //standard timeout, reccomend atleast 20000
#define LOOP_DELAY 500

/*Default Pin Values*/
#define RSX_PIN 12
#define RSY_PIN 11
#define LSX_PIN 9
#define LSY_PIN 10

#define GEAR_PIN 8
#define AUX_PIN 7

/* Default output range for axis/knob output*/
#define STD_HIGH_OUTPUT 1000
#define STD_LOW_OUTPUT -1000

/*Default Offset Values*/
//RSX Receiver ch 1
#define RSX_LOW_OFFSET 1842  //Full left
#define RSX_HIGH_OFFSET 1074  //Full right

//RSY Receiver ch 2
#define RSY_HIGH_OFFSET 1855  //Full left
#define RSY_LOW_OFFSET 1123  //Full right

//LSX Receiver ch 4
#define LSX_HIGH_OFFSET 1862 
#define LSX_LOW_OFFSET 1079  

//LSY Receiver ch 3
#define LSY_HIGH_OFFSET 1894  //Full left
#define LSY_LOW_OFFSET 1117  //Full right

//Gear receiver ch 5
#define GEAR_HIGH_OFFSET 1500
#define GEAR_LOW_OFFSET 1089

//AUX knob receiver ch 6
#define AUX_HIGH_OFFSET 1891
#define AUX_LOW_OFFSET 1073

//channel 7 is affected by everything!!!!!!


//Channel defining for testing
/*RCAxisChannel *RSX;
RCAxisChannel *RSY;
RCAxisChannel *LSY;
RCAxisChannel *LSX;

RCSwitchChannel *gearSwitch;
RCAxisChannel *AUX;*/


//Initiates a default RCAxisChannel
RCAxisChannel *initRCAxisChannel(int pinIn, int highOffset, int lowOffset){
  RCAxisChannel *ch;
  
  ch = (RCAxisChannel *)malloc(sizeof(RCAxisChannel));
  ch->pin=pinIn;
  
  ch->highOffset=highOffset;
  ch->lowOffset=lowOffset;
  
  //Sets the default output range
  ch->highOutput=STD_HIGH_OUTPUT;
  ch->lowOutput=STD_LOW_OUTPUT;
  
  ch->outputLevel=0;
  
  return ch;
} 

//Initiates a default RC Switch Channel
RCSwitchChannel *initRCSwitchChannel(int pinIn, int highOffset, int lowOffset){
  RCSwitchChannel *ch;
  
  ch = (RCSwitchChannel *)malloc(sizeof(RCSwitchChannel));
  ch->pin=pinIn;
  
  ch->highOffset=highOffset;
  ch->lowOffset=lowOffset;
  
  ch->numberOfOptions=2;
  ch->outputSelection=0;
  
  
  return ch;
} 

//setups channels for testing
RC *initRC(){
  
  RC *rc = (RC *)malloc(sizeof(RC));
  
  rc->RSX = initRCAxisChannel(RSX_PIN, RSX_HIGH_OFFSET, RSX_LOW_OFFSET);
  pinMode(rc->RSX->pin, INPUT);
  
  rc->RSY = initRCAxisChannel(RSY_PIN, RSY_HIGH_OFFSET, RSY_LOW_OFFSET);
  pinMode(rc->RSY->pin, INPUT);
  
  rc->LSX = initRCAxisChannel(LSX_PIN, LSX_HIGH_OFFSET, LSX_LOW_OFFSET);
  pinMode(rc->LSX->pin, INPUT);
  
  rc->LSY = initRCAxisChannel(LSY_PIN, LSY_HIGH_OFFSET, LSY_LOW_OFFSET);
  pinMode(rc->LSY->pin, INPUT);
  
  rc->AUX = initRCAxisChannel(AUX_PIN, AUX_HIGH_OFFSET, AUX_LOW_OFFSET);
  setOutputRange(rc->AUX,0,1000);
  pinMode(rc->AUX->pin, INPUT);
  
  rc->gearSwitch = initRCSwitchChannel (GEAR_PIN, GEAR_HIGH_OFFSET, GEAR_LOW_OFFSET);
  pinMode(rc->gearSwitch->pin, INPUT);
  
  return rc;
}


//Internal - Used to update all channels
void updateControllerValues(RC *rc){
  updateAxisPulse(rc->RSX);
  updateAxisPulse(rc->RSY);
  updateAxisPulse(rc->LSX);
  updateAxisPulse(rc->LSY);
  updateAxisPulse(rc->AUX);
  updateSwitchPulse(rc->gearSwitch);
}

//Sets a custom output range for axis/knob channels
void setOutputRange(RCAxisChannel *ch, int low, int high){
  ch->highOutput=high;
  ch->lowOutput=low;
}

/*Updates Channel from Pulses*/
void updateAxisChannelOutput(RCAxisChannel *ch){
  updateAxisPulse(ch);
}

void updateSwitchChannelOutput(RCSwitchChannel *ch){
  updateSwitchPulse(ch);
}

/*Updates channel then returns channel output*/
int getAxisOutput(RCAxisChannel *ch){
  updateAxisPulse(ch);
  return ch->outputLevel;
}

int getSwitchOutput(RCSwitchChannel *ch){
  updateSwitchPulse(ch);
  return ch->outputSelection;
}

/*Updates Channel and returns output,
  But output is limited to the channel's range*/
int getBoundedOutput(RCAxisChannel *ch){
  updateAxisPulse(ch);
  
  if(ch->outputLevel>ch->highOutput)
    return ch->highOutput;
    
  else if(ch->outputLevel<ch->lowOutput)
    return ch->lowOutput;
    
  else
    return ch->outputLevel;
}
  
/*Updates axis channel from pulse*/
void updateAxisPulse(RCAxisChannel *ch){
  unsigned long pulseLength = pulseIn(ch->pin, HIGH, STD_TIMEOUT);
  ch->outputLevel=map(pulseLength, ch->lowOffset, ch->highOffset, ch->lowOutput,ch->highOutput);
}

/*Updates switch channel from pulse*/
void updateSwitchPulse(RCSwitchChannel *ch){
  unsigned long pulseLength = pulseIn(ch->pin, HIGH, STD_TIMEOUT);
  ch->outputSelection=map(pulseLength, ch->lowOffset, ch->highOffset, 0,ch->numberOfOptions-1);
}

/*Prints stick cords. from the two axis channels
void printStickCords(RCAxisChannel *X, RCAxisChannel *Y){
  Serial.print("(");
  Serial.print(X->outputLevel);
  Serial.print(", ");
  Serial.print(Y->outputLevel);
  Serial.print(")\n");
}

void printSwitchState(RCSwitchChannel *ch){
  Serial.print("[");
  Serial.print(ch->outputSelection);
  Serial.print("]\n");
  
}

void printAll(RC *rc){
  printAll(rc->RSX,rc->RSY,rc->LSX,rc->LSY,rc->AUX,rc->gearSwitch);
}

void printAll(RCAxisChannel *RSX, RCAxisChannel *RSY, RCAxisChannel *LSX, RCAxisChannel *LSY, RCAxisChannel *AUX, RCSwitchChannel *gear){
  Serial.print("(");
  Serial.print(RSX->outputLevel);
  Serial.print(", ");
  Serial.print(RSY->outputLevel);
  Serial.print(")       (");
  
  Serial.print(LSX->outputLevel);
  Serial.print(", ");
  Serial.print(LSY->outputLevel);
  Serial.print(")       (");
  
  Serial.print(AUX->outputLevel);
  Serial.print(")\t");
  
  Serial.print("[");
  Serial.print(gear->outputSelection);
  Serial.print("]\n");
}
*/
