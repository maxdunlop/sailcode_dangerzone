#ifndef RCSTRUCT_H
#define RCSTRUCT_H
#include <Arduino.h>
//For channels that connect to an RC stick axis or knob
typedef struct RCAxisChannel{
  int pin;            //The input pin
  
  int highOffset;    //The max length of the pulse
  int lowOffset;      //The min length of the pulse
  
  int highOutput;  //Range for output
  int lowOutput;
  
  int outputLevel;  //Current output level relative to a given range
}RCAxisChannel;

//For channels that connect to an RC switch
typedef struct RCSwitchChannel{
  int pin;
  
  int highOffset;
  int lowOffset;
  
  int numberOfOptions;  //Number of switch options
  int outputSelection;  //Stores which option is currently selected
}RCSwitchChannel;

typedef struct RC{
  RCAxisChannel *RSX;
  RCAxisChannel *RSY;
  RCAxisChannel *LSY;
  RCAxisChannel *LSX;

  RCSwitchChannel *gearSwitch;
  RCAxisChannel *AUX;
}RC;


RCAxisChannel *initRCAxisChannel(int pinIn, int highOffset, int lowOffset);

//Initiates a default RC Switch Channel
RCSwitchChannel *initRCSwitchChannel(int pinIn, int highOffset, int lowOffset);


//setups channels for testing
RC *initRC();




//Internal - Used to update all channels
void updateControllerValues(RC *rc);

//Sets a custom output range for axis/knob channels
void setOutputRange(RCAxisChannel *ch, int low, int high);

/*Updates Channel from Pulses*/
void updateAxisChannelOutput(RCAxisChannel *ch);

void updateSwitchChannelOutput(RCSwitchChannel *ch);

/*Updates channel then returns channel output*/
int getAxisOutput(RCAxisChannel *ch);

int getSwitchOutput(RCSwitchChannel *ch);

/*Updates Channel and returns output,
  But output is limited to the channel's range*/
int getBoundedOutput(RCAxisChannel *ch);
  
/*Updates axis channel from pulse*/
void updateAxisPulse(RCAxisChannel *ch);

/*Updates switch channel from pulse*/
void updateSwitchPulse(RCSwitchChannel *ch);

/*Prints stick cords. from the two axis channels*/
void printStickCords(RCAxisChannel *X, RCAxisChannel *Y);

void printSwitchState(RCSwitchChannel *ch);

void printAll(RC *rc);

void printAll(RCAxisChannel *RSX, RCAxisChannel *RSY, RCAxisChannel *LSX, RCAxisChannel *LSY, RCAxisChannel *AUX, RCSwitchChannel *gear);

#endif

