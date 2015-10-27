

#include <avr/sleep.h>
#include <EEPROM.h>

volatile int sleep_count = 0 ; 


////
int ledPin = 13 ;
int tempPin = A1 ;
int lightPin = A6 ;
int switchPin= 2 ;
int value1,value2 ;


int buffTotal = 200 ;
int buffPointer = 0 ;
int slot = 0;


///

void setup( ) {

	Serial.begin( 9600 ) ;

	pinMode(ledPin, OUTPUT);
	pinMode(switchPin, INPUT);
	digitalWrite(switchPin, HIGH);

	//Serial.println(EEPROM.length(),DEC);
	//delay(1000);

	buffPointer = EEPROM.read( 0 ) ;
	if( buffPointer > buffTotal ) buffPointer = 0 ;
	delay( 100 ) ;

	watchdogOn( ); 

}

byte getSlot( int bf ) {

	return( (  bf * 4 ) + 1 ) ;

}

void deepSleep( ) {

	for( int i = 0 ; i < 50 ; i++ ) {

		goToSleep( ) ; 
		
	}

}

void readSensors( ) {

	value1 = analogRead( tempPin ) ;
	value2 = analogRead( lightPin) ;	

}

void saveData( ) {

	slot = getSlot( buffPointer ) ;

	EEPROM.write( slot + 0 , highByte( value1 ) ) ;
	EEPROM.write( slot + 1 , lowByte( value1 ) ) ;
	EEPROM.write( slot + 2 , highByte( value2 ) ) ;
	EEPROM.write( slot + 3 , lowByte( value2 ) ) ;

	buffPointer++ ;

	if( buffPointer == buffTotal ) buffPointer = 0 ;

	EEPROM.write( 0 , buffPointer ) ;

}

void sendData( ) {

	Serial.println("#");		

	for( int i = 0 ; i < buffTotal ; i++ ) {

		slot = getSlot( i ) ;

		value1 = ( EEPROM.read( slot ) * 256 ) + EEPROM.read( slot + 1 ) ;

		value2 = ( EEPROM.read( slot + 2 ) * 256 ) + EEPROM.read( slot + 3 ) ;

		if( i == buffPointer ) {

			Serial.print( "!" ) ;

		} else { 

			Serial.print( "_" ) ;

		}

		Serial.print(",");

		Serial.print(i);
		Serial.print(",");
		Serial.print(value1);
		Serial.print(",");
		Serial.println(value2);		

	}


}

void loop( ) {

	if( digitalRead( switchPin ) == LOW ) {

		deepSleep( ) ;

		readSensors( ) ;

		saveData( ) ;

		delay( 10 ) ;

	} else {

		sendData( ) ;

		digitalWrite(ledPin, HIGH);
		delay( 100 ) ;
		digitalWrite(ledPin, LOW);	

		delay( 1000 ) ;

	}	



}

void goToSleep( )   {

	set_sleep_mode( SLEEP_MODE_PWR_DOWN ) ;
	sleep_enable( ) ;
	sleep_mode( ) ;

	sleep_disable(); // Disable sleep mode after waking.
                     
}

void watchdogOn() {
  
	MCUSR = MCUSR & B11110111;
	  
	WDTCSR = WDTCSR | B00011000; 

	WDTCSR = B00100001;

	// Enable the watchdog time interupt.
	WDTCSR = WDTCSR | B01000000;
	MCUSR = MCUSR & B11110111;

}

ISR( WDT_vect ) {

	sleep_count ++ ;

}



