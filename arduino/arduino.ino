#include <pololu_servo.h>
#include <ashcon.h>
#include <aprintf.h>
#include <pololu_controller.h>
#include <RCstructs.h>

#include "cmdline_functions.h"

static const int pololu_reset_pin = 3;

ashcon* Console;
polcore* motor;
polcore* mast;
polcore* sail;
RC* rc;
PololuMSC* rudder;

void setup() {
    Serial.begin(19200);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial3.begin(4800);

    pconInitialise( &mast, &Serial2, 12 );
    pconInitialise( &sail, &Serial2, 13 );
    pconInitialise( &motor, &Serial2, 13 );

    rudder = new PololuMSC(&Serial1, NULL);

    Console = new ashcon( &Serial );
    Console->user_function_register( "about", &uabout );
    Console->user_function_register( "pol", &upol );
    Console->user_function_register( "motorGo", &umotorGo );
    Console->user_function_register( "m", &umotorGo ); //For brevity, since we'll be typing it a lot
    Console->user_function_register( "mon", &umon);
    Console->user_function_register( "serv", &uservo);
    Console->user_function_register( "rc", &urcpollall);
	Console->user_function_register( "startRC", &ustartRCControl);
  Console->user_function_register( "help", &uhelp);

    uinitRC( NULL );
}

void loop() {
    Console->command_prompt();
}
