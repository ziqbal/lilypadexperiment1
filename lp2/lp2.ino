



////
int ledPin = 13 ;
int tempPin = A1 ;
int lightPin = A6 ;
int switchPin= 2 ;
int value1,value2 ;



///

void setup( ) {

	Serial.begin( 9600 ) ;

	pinMode(ledPin, OUTPUT);
	pinMode(switchPin, INPUT);
	digitalWrite(switchPin, HIGH);

}

void readSensors( ) {

	value1 = analogRead( tempPin ) ;
	value2 = analogRead( lightPin) ;	

}

void sendData( ) {

	Serial.print( value1 ) ;
	Serial.print( "," ) ;
	Serial.println( value2 ) ;		

}

void loop( ) {

	readSensors( ) ;
	sendData( ) ;

	delay( 100 ) ;




}




