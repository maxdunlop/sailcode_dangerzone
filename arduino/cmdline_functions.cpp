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
  int pololuId = 0;
  polcore *motorDev;
  sscanf(argv[1], "%c", &motorId);
  sscanf(argv[2], "%d", &motorSpeed);

  if (motorId == 'm') {
    motorDev = mast;
  } else if (motorId == 's') {
    motorDev = sail;
  }else if (motorId == 'p') {
	sscanf(argv[2], "%d", &pololuId);
	sscanf(argv[3], "%d", &motorSpeed);
  } else {
    Console->out->printf("Unknown device.  First param must be \"m\" \
	\"s\", or \"p\".\n");
    return 1;
  }
  
  if (motorDev == mast || motorDev == sail) {
  sscanf(argv[2], "%d", &motorSpeed);
    
  pconGenMotorGo( motorDev, pcon_buffer_to_dev, abs(motorSpeed),
		  motorSpeed>0 ? PCON_FORWORD : PCON_BACKWORD );
  pconSendCommandBuffer( motorDev );
  }

  if (motorId == 'p') {
	  rudder->setPosition(pololuId, motorSpeed);
  }

  if (motorDev == mast) {
    Console->out->printf("Mast");
  } else if (motorDev == sail) {
    Console->out->printf("Sail");
  } else if (motorId == 'p') {
	  Console->out->printf("Pololu #%d", pololuId);
  } else{
    Console->out->printf("Internal error: unknown device object!");
    return 1;
  }
  Console->out->printf(" motor set to %d.\n", motorSpeed);
  
  return 0;
}

int ustartRCControl(char* argv[]) {
	int leftStickX, rightStickX, rudderAngle, sailSpeed, offSwitch;
	char input = 'a';
      
	while (input != 'q') {

        offSwitch = getSwitchOutput(rc->gearSwitch);
        if (offSwitch == 1) {
        Console->out->printf("Emergency Off Switch Activated!\n");    
        break;
        } else if (offSwitch == -2) {
        Console->out->printf("RC Turned Off!\n");
        break;
        }

		leftStickX = getBoundedOutput(rc->LSX);
		rightStickX = getBoundedOutput(rc->RSX);

		rudderAngle = (leftStickX / 8)+125;
		sailSpeed = abs(rightStickX)>50 ? rightStickX / 10 : 0;

		rudder->setPosition(7,rudderAngle);
		pconGenMotorGo( sail, pcon_buffer_to_dev, abs(sailSpeed),
						sailSpeed>0 ? PCON_FORWORD : PCON_BACKWORD );
		pconSendCommandBuffer( sail );   
	
		Console->out->printf( "R:%d\tS:%d\n", rudderAngle, sailSpeed);
        if(Serial.available()) input = Serial.read();
	}

	pconGenMotorGo( sail, pcon_buffer_to_dev, 0, PCON_FORWORD);
	pconSendCommandBuffer( sail );     

	return 0;
	
}

int uhelp(char* argv[]){
  Console->out->printf( "about\n" );
  Console->out->printf( "pol\n");
    Console->out->printf( "motorGo\n");
    Console->out->printf( "m\n" ); //For brevity, since we'll be typing it a lot
    Console->out->printf( "mon\n");
    Console->out->printf( "serv\n");
    Console->out->printf( "rc\n");
	Console->out->printf( "startRC\n");
}


