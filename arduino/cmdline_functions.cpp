#include "cmdline_functions.h"

int uinitRC(char* argv[]){
	rc = initRC();
}

int urcpollall(char* argv[]){
    char c = '1';

    while( c != 'q' ) {
        updateControllerValues(rc);
        Console->out->printf("LS: (%d,%d) RS: (%d,%d) SW[%d] AUX[%d]\n",
                getAxisOutput(rc->LSX),
                getAxisOutput(rc->LSY),
                getAxisOutput(rc->RSX),
                getAxisOutput(rc->RSY),
                getSwitchOutput(rc->gearSwitch),
                getAxisOutput(rc->AUX) );

        if( Serial.available() ) c = Serial.read();
    }
}

int uabout(char* argv[]) {
    // Cool little ascii art made with a program called figlet, gotta
    // escape the special characters though

    // Remember, printf has a 80 char limit at a time
    // This eats a tonne of sram, TODO switch this into a PROGMEM string
    Console->out->printf("  ___|    \\    ____| |     ____| _ \\   _ \\"
            "___| ____| \\ \\  / |     \n\r");
    Console->out->printf(" |       _ \\   __|   |     |    |   | |   | |"
            "     __|    \\  /  |     \n\r");
    Console->out->printf(" |   |  ___ \\  |     |     __|  |   | __ <  |"
            "     |         \\  |     \n\r");
    Console->out->printf("\\____|_/    _\\_____|_____|_|   \\___/ _| \\_"
            "\\\\____|_____|  _/\\_\\_____| \n\r\n\r\n\r");

    Console->out->printf("AshCon Version 0.02 \n\r");

    return 0;
}

// The possibilities for functions that respond to command line arguments are
// pretty much endless
int upol(char* argv[]) {
    uint8_t value;
    sscanf( argv[1], "%d",&value );
    
    Console->out->printf("POWER: %d\r\n", value );

    pconGenMotorGo( motor, pcon_buffer_to_dev, value, PCON_FORWORD );
    pconSendCommandBuffer( motor );
}

int umon(char* argv[]) {
    Stream* ser_line;
    uint8_t value;
    sscanf( argv[1], "%d",&value);
    
    //Console->out->printf("MONITER: %d\n", value);
    
    if(value == 1) 
      ser_line = &Serial1;  
    else if(value == 2) 
      ser_line = &Serial2;
    else if(value == 3)
      ser_line = &Serial3;
  
    char c = '1';
    
    while(c != 'q') {
       if(ser_line->available() > 0) {
          //Console->out->printf("%c", ser_line->read());
          Serial.print( (char) ser_line->read());
       }
       
       if(Serial.available())
         c = Serial.read();
    }
}

int uservo(char* argv[]) {
  while( 1 ) {
        rudder->setPosition(7, 250);
        delay(500);
        rudder->setPosition(7, 0);
        delay(500);
  }
}

int umotorGo(char* argv[]) {
  char motorId;
  int motorSpeed;
  polcore *motorDev;
  sscanf(argv[1], "%c", &motorId);
  sscanf(argv[2], "%d", &motorSpeed);

  if (motorId == 'm') {
    motorDev = mast;
  } else if (motorId == 's') {
    motorDev = sail;
  } else {
    Console->out->printf("Unknown device.  First param must be \"m\" or \"s\"");
    return 1;
  }
  
  pconGenMotorGo( motorDev, pcon_buffer_to_dev, abs(motorSpeed),
		  motorSpeed>0 ? PCON_FORWORD : PCON_BACKWORD );
  pconSendCommandBuffer( motorDev );

  if (motorDev == mast) {
    Console->out->printf("Mast");
  } else if (motorDev == sail) {
    Console->out->printf("Sail");
  } else {
    Console->out->printf("Internal error: unknown device object!");
    return 1;
  }
  Console->out->printf(" motor power set to %d.\n", motorSpeed);
  
  return 0;
}
