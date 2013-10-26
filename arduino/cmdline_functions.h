#ifndef CMDLINE_FUNCTIONS_H
#define CMDLINE_FUNCTIONS_H value

#include <Arduino.h>
#include <ashcon.h>
#include <pololu_controller.h>
#include <pololu_servo.h>
#include <RCstructs.h>

// Global instances declared in the main sketch that need to be used by the
// command line functions
extern ashcon* Console;
extern polcore* motor;
extern polcore* sail;
extern polcore* mast;
extern RC* rc;
extern PololuMSC* rudder;

// Prototype declarations for cmdline functions
int uabout(char* argv[]);
int upol(char* argv[]);
int umon(char* argv[]);
int uservo(char* argv[]);
int uinitRC(char* argv[]);
int urcpollall(char* argv[]);
int umotorGo(char* argv[]);
int ustartRCControl(char* argv[]);
int uhelp(char* argv[]);
#endif
