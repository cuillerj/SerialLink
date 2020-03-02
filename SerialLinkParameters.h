/*
#define esp8266Shield if a wifi shield uno type is used in order
	in this case serial0 link is used even with Atmega 
Define for ATmega2560 and ATmega1280 which serial link to use 
Can be serial1 serial2 or serial3
if esp8266Shield this serial is used to print debug
*/
#define esp8266Shield       // #define esp8266Shield  uncomment only when using a shield
#define serial2
//#define debugSerialLink     // uncomment to debug