////////////////////////////////////////////////////////////////////////
//                       ACCESS CONTROL v1.1.2                        //
////////////////////////////////////////////////////////////////////////
#define ID_1 'A'
#define ID_2 'C'
#define ID_3 '1'
#define ID_4 '1'

/*
Zan Hecht - 11 February 2025
http://zansstuff.com/access-control

Requires forked Wiegand-Protocol-Library-for-Arduino from:
https://github.com/zanhecht/Wiegand-Protocol-Library-for-Arduino
(forked from https://github.com/monkeyboard/Wiegand-Protocol-Library-for-Arduino)

This sketch was tested with an HID RPK40 MultiClass reader, but should work
with any card reader or pin pad that uses the wiegand protocol. The reader
should have input wires for a buzzer, red LED, and green LED. Output to the
door lock and doorbell should be via relays or a relay shield.

The Wiegand reader should be powered with common ground to the arduino, and
with DATA0 (typically the green wire) connected to Pin 2 and DATA1 (typically
the white wire) connected to pin 3.

All functions can be done interactively using the serial port or via the
keypad. To disable serial functions, change the line in the CONFIGURATION
section of the code to "#define SERIAL_ENABLE 0".

INSTRUCTIONS
------------

The first time the sketch starts up (or after a reinitialization), the
configuration code will be reset to 123456 on the pin pad (or whatever value
is defined in the CONFIGURATION section of the code). If the PIN_LENGTH is
different from the length of the configuration code, 0s will be added or
digits will be removed from the beginning of the configuration code.

### Normal mode (three amber beeps/flashes, then no light)
Enter any valid code or card (other than the configuration code/card) to
unlock the door. To cancel entering a code, hit "#".

If a valid card/code is entered, the green light will turn on and the door
will unlock for 5 seconds (or as defined by UNLOCK_DURATION in the
CONFIGURATION section of the code).

If an incorrect code/card is entered, the reader will beep and flash red for
one second. No codes/cards can be entered during this time. If an incorrect
card/code is entered again, this period will double to 2 seconds, then to 4
seconds, and then to 8 seconds. The initial lockout time and the maximum
lockout time can be changed by defining LOCKOUT_DURATION and
LOCKOUT_MAX_MULTIPLIER in the CONFIGURATION section of the code. The lockout
period can be disabled by setting LOCKOUT_DURATION to 0, and the limit on the
maximum lockout time can be removed by setting LOCKOUT_MAX_MULTIPLIER to 0.

Hit "*" to ring the doorbell. The green light will come on for half a second.
This time can be changed by defining DOORBELL_DURATION in the CONFIGURATION
section of the code. If DOORBELL_DURATION is set to 0, "*" will clear
partially entered codes but not ring the doorbell. You can also wire a physical
doorbell button between DOORBELL_BUTTON_PIN and ground.

Enter the configuration code or card to enter configuration mode. The reader
will beep/flash amber twice. The reader will automatically exit from
configuration mode after a set amount of time with no input (defaults to three
times the unlock duration, determined by TIMEOUT_DELAY_MULTIPLIER in the
CONFIGURATION section of the code).

### Configuration Mode (Two amber flashes, then solid amber)

> Configuration mode will print all stored codes over the serial port (see the
> Unhide/Hide section below). 26- and 34-bit codes are split so that the first
> 3-5 characters are the facility code (the top 8-16 bits) and the last 5
> digits are the ID number (the bottom 16 bits).
> 
> * Press/Type "1" to Add a code/card
> * Press/Type "2" to Delete a code/card
> * Press/Type "3" to Change the configuration code/card
> * Type "4" to Delete by slot (via serial only)
> * Type "0" to Unhide/Hide codes (via serial only)
> * Press/Type "#" to Exit to normal mode
> 
> In general, the following signals are used:
> * Two beeps and green flashes: operation successful
> * Two beeps and red flashes: operation failed
> * Two beeps and amber flashes: entering Configuration Mode
> * Three beeps and amber flashes: entering Normal Mode
> 
> #### 1. Add (flashing green)
>
> > Enter a code or scan a card to add it to the unlock codes. Hit "#" to
> > return to Configuration Mode (reader will beep/flash amber twice).
> > 
> > If the code/card is entered successfully, the reader will beep/flash green
> > twice.
> >
> > If the code/card is invalid or already exists, the reader will beep/flash
> > red twice.
> > 
> > Once a code/card is entered, you can enter another code/card, or hit "#"
> > to return to Configuration Mode.
> 
> #### 2. Delete (flashing red)
> 
> > Enter a code or scan a card to remove it from the unlock codes/cards. You
> > cannot remove code/card #1 (the configuration code/card). Hit "#" to
> > return to Configuration Mode (reader will beep/flash amber twice).
> > 
> > If the code/card is removed successfully, the reader will beep/flash green
> > twice.
> > 
> > If the code/card is invalid or doesn't exist, the reader will beep/flash
> > red twice.
> > 
> > Once a code/card is entered, you can enter another code/card, or hit "#"
> > to return to Configuration Mode.
> 
> #### 3. Change configuration code/card (flashing amber)
>
> > Enter a code or scan a card to set it as the configuration code/card.
> > 
> > If the code/card is entered successfully, the reader will beep/flash green
> > three times and return to normal mode.
> >
> > If the code/card is invalid or unchanged, the reader will beep/flash red
> > three times and return to normal mode.
>
> #### 4. Delete by slot # (serial only)
>
> > This mode can only be entered when connected via serial monitor (or via
> > bluetooth). Enter a slot number from the list to delete the code.
> > 
> > If the slot number is valid, the code will be deleted and a confirmation
> > will be shown on the serial output. Otherwise, an error message will be
> > shown.
> >
> > This mode does not produce any beeps or flashes on the reader.
>
> #### 0. Unhide/Hide codes (serial only)
>
> > By default, entered codes are not displayed over the serial interface to
> > prevent a third party from viewing codes as they are entered if you have a
> > wireless serial interface on your board. If you do not have a wireless
> > interface and the board is located in a secure area, you can change this
> > default behavior by setting the HIDE_CODES variable in the CONFIGURATION
> > section of the code to 0. Entering "0" in configuration mode via the
> > serial interface will toggle this on or off.
> >
> > If SERIAL_ENABLE and HIDE_CODES are set to 1 and this option is toggled
> > to "Unhide", the reader will continuously flash amber to warn that any
> > entered codes are potentially visible.

### Reinitialize
 
> If you forget your configuration code, or otherwise want to reset all stored
> codes, short ground to pin 12 (or whatever pin is set to INIT_BUTTON_PIN in
> the CONFIGURATION section of the code). This can be connected to a button if
> the arduino is in a secure location.
> 
> When pin 12 is grounded, the reader will beep and flash amber. After the pin
> has been grounded for 10 seconds, the flashing will change to green. Reset
> or power-cycle the arduino, and it will delete all codes on power-up and
> reset the configuration code to 123456 (or whatever value is defined in the
> CONFIGURATION section of the code).

TO DO
-----
* Store configuration in EEPROM in an unsigned long
  * Parity Even (1 bit)
  * Hide Codes (1 bit)
  * Pin Length (4 bits)
  * Timeout Unlock Time Multiplier (4 bits)
  * Lockout Max Multiplier (4 bits)
  * Initial Lockout Time (4 bits)
  * Doorbell Duration (tenths of a second) (6 bits)
  * Unlock Time (seconds) (7 bits)
  * Parity Odd (1 bit)
*/

/////////////////////////////CONFIGURATION//////////////////////////////

// Set this to 1 to force EEPROM initialization on power up.
// This will reset the configuration code and wipe all stored codes.
#define INITIALIZE 0

//Default configuration code. Change immediately.
#define CONF_CODE 123456

// Enable Serial output (0 to disable, 1 to enable)
#define SERIAL_ENABLE 1

// Hide codes by default in serial output to prevent sniffing
// when using bluetooth (0 to disable masking, 1 to enable masking)
#define HIDE_CODES 1

//Define IO pins for door striker, led, buzzer, and doorbell.
//DATA0 is Pin 2 (green wire) and DATA1 is pin 3 (white wire).
#define DOOR_PIN 4
#define DOORBELL_PIN 5
#define BUZZER_PIN 6 //yellow wire
#define RED_LED_PIN 7 //brown wire
#define GREEN_LED_PIN 10 //orange wire
#define DOORBELL_BUTTON_PIN 11
#define INIT_BUTTON_PIN 12

//Time the door is held unlocked, in seconds
#define UNLOCK_DURATION 5

//Time to hold doorbell button, in milliseconds (0 to disable * as doorbell).
#define DOORBELL_DURATION 500

//Default reader lockout duration if bad card/pin used, in seconds (0 to
//disable).
#define LOCKOUT_DURATION 1

//Lockout time will double with each consecutive bad card/pin
//up to the limit specified here times the LOCKOUT_DURATION (0 for no maximum).
//For example, a LOCKOUT_DURATION of 2 and a LOCKOUT_MAX_MULTIPLIER of 4 would
//result in a maximum lockout time of 8 seconds.
#define LOCKOUT_MAX_MULTIPLIER 8

//Number of digits in PIN codes
#define PIN_LENGTH 6

//How long before partially entered PINs time out, as a multiple of
//UNLOCK_DURATION (0 to disable). Config mode will wait 4 times as long before
//exiting.
#define TIMEOUT_DELAY_MULTIPLIER 3

//Set to 1 to use CRC for EEPROM error checking
//Set to 0 to use static version ID for for EPPROM error checking
#define USE_CRC 0

/////////////////////////////////SETUP//////////////////////////////////

#define ENTER_KEY 0x1b
#define ESC_KEY 0x0d

#include <EEPROM.h>
#include <Wiegand.h>

WIEGAND wg;

// Assorted function prototypes
unsigned long assemblePIN(unsigned long code, byte pinLength=PIN_LENGTH);

// Global Varibles and Constants
unsigned long unlockEnd, lockoutEnd, doorBellEnd, initEnd;
unsigned long pin, nextPattern, timeoutTime;
unsigned long timeoutDuration, lockoutDuration, lockoutMax;
byte pinCount, serialAvailable, hideCodes, patternPosition;
byte indicators, pattern, repeat;
int mode;
const int ulSize = sizeof(unsigned long);
const int eLength = EEPROM.length();

void setup() {
	//set global variables
	unlockEnd=0;
	lockoutEnd=0;
	doorBellEnd=0;
	initEnd=0;
	pin=0;
	nextPattern=0;
	timeoutTime=0;
	timeoutDuration=TIMEOUT_DELAY_MULTIPLIER*UNLOCK_DURATION;
	lockoutDuration=LOCKOUT_DURATION;
	lockoutMax=LOCKOUT_MAX_MULTIPLIER*LOCKOUT_DURATION;
	pinCount = 0;
	serialAvailable = 0;
	hideCodes = HIDE_CODES;
	patternPosition=0b00000001;
	mode = 0;

	indicators = 0b000;		// Bit2 = buzzer, Bit1 = green, Bit0 = red
	pattern = 0b00000000;	// On-off pattern in ~1/8 second increments
	repeat = 0;						// Number of times to repeat pattern

	//set inputs
	pinMode(DOORBELL_BUTTON_PIN, INPUT_PULLUP);
	pinMode(INIT_BUTTON_PIN, INPUT_PULLUP);
	
	//set output pins
	pinMode(DOOR_PIN, OUTPUT);
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(DOORBELL_PIN, OUTPUT);
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);

	digitalWrite(DOOR_PIN, HIGH);
	digitalWrite(DOORBELL_PIN, HIGH);
	digitalWrite(RED_LED_PIN, HIGH);
	digitalWrite(GREEN_LED_PIN, HIGH);
	
	if (SERIAL_ENABLE) {
		Serial.begin(9600);
		Serial.println(F("Serial begin"));
		Serial.print(F("EEPROM size: ")); Serial.println(eLength);
	}
	
	//Check EEPROM integrity
	unsigned long slot0;
	EEPROM.get(0, slot0);
	if ((eepromCRC() != slot0) || INITIALIZE) {
		if (SERIAL_ENABLE) { Serial.print(F("CRC fail. Initializing EEPROM. Configuration code: ")); Serial.println(CONF_CODE); }
		for (int i=ulSize*2; i<eLength; ++i) { EEPROM.update(i, 0); }
		EEPROM.put(ulSize, CONF_CODE);
		slot0=eepromCRC();
		EEPROM.put(0, slot0);
	}
	
	if(SERIAL_ENABLE) { Serial.print(F("CRC ")); Serial.print(slot0, DEC); Serial.println(F(" OK.")); }

	//Print config
	Serial.print(F("Hide codes: "));Serial.print(HIDE_CODES);
	Serial.print(F("Unlock duration: "));
	Serial.print(UNLOCK_DURATION);Serial.println(F(" seconds."));
	Serial.print(F("Doorbell duration: "));
	Serial.print(DOORBELL_DURATION);Serial.println(F(" milliseconds."));
	Serial.print(F("Lockout: "));
	Serial.print(lockoutDuration);Serial.println(F(" seconds."));
	Serial.print(F("Lockout max: "));
	Serial.print(lockoutMax);Serial.println(F(" seconds."));
	Serial.print(F("Timeout: "));
	Serial.print(timeoutDuration);Serial.println(F(" seconds."));
	Serial.print(F("Pin length: "));
	Serial.print(PIN_LENGTH);Serial.println(F(" digits."));
	
	//Make sure confCode is valid
	unsigned long confCode;
	EEPROM.get(ulSize, confCode);
	int n = 0;
	for(unsigned long c = confCode; c > 0; c /= 10) { n++; }
	if ( n > PIN_LENGTH ) {
		if(SERIAL_ENABLE) {
			Serial.print(F("Configuration code trimmed to last "));
			Serial.print(PIN_LENGTH); Serial.println(F(" digits."));
		}
		EEPROM.put( ulSize, (confCode % (unsigned long)pow(10, PIN_LENGTH)) );
	}

	//begin reading weigand data
	wg.begin();

	enterNormal(0b111); //amber beep
}
//////////////////////////////////LOOP//////////////////////////////////
void loop() {
	unsigned long now = millis();
	if (lockoutEnd && (now > lockoutEnd) ) {
		lockoutEnd = 0; //end lockout
		if(SERIAL_ENABLE) { Serial.println(F("Lockout ended.")); }
	}

	if (timeoutDuration && timeoutTime) { // If timeout enabled and PIN reset timer is set
		if (!mode) { // If in normal mode
			if (now > (timeoutTime + ((unsigned long)(timeoutDuration) * 1000))) { // Pin timeout has passed
				assemblePIN(ESC_KEY);
			}
		} else if (now > (timeoutTime + ((unsigned long)(timeoutDuration) * 4000))) { // 4 times pin timeout has passed
			timeoutTime = 0;
			enterNormal(0b111); //amber beep
		}
	}

	bool wgAvailable = wg.available();
	bool doorbellButton = (digitalRead(DOORBELL_BUTTON_PIN) == LOW);
 
	if ( (wgAvailable || (SERIAL_ENABLE && Serial.available())) && !lockoutEnd ){
		unsigned long code = 0;
		byte codeType = 0;
		
		noInterrupts();
		
		if (wgAvailable) {
			codeType = wg.getWiegandType();
			serialAvailable = 0;
			code = wg.getCode();
			if (SERIAL_ENABLE && (codeType == 4 || codeType == 8)) {
				if (hideCodes) {
					Serial.print(F("W"));
					Serial.println(codeType);
				} else {
					Serial.println(code);
				}
			}
		} else if (SERIAL_ENABLE && Serial.available()) {
			//Serial console or bluetooth input
			serialAvailable = 0;
			while (Serial.available()	> 0) {
				byte serialRead = Serial.read();
				if (serialRead == 42) { // ASCII *
					code = ENTER_KEY;
					codeType = 4;
				} else if (serialRead == 35) { // ASCII #
					code = ESC_KEY;
					codeType = 4;
				} else if (serialRead >= 48 && serialRead <= 57) { // digit
					serialAvailable++;
					code = (code * 10) + (serialRead - 48);
				}
				interrupts();
				delay(25);
				noInterrupts();
			}
			if (!codeType) { //determine input size if not overridden
				if (serialAvailable == 1) {
					codeType = 4;
				} else if (serialAvailable > 1 && serialAvailable <= PIN_LENGTH) {
					codeType = 0;
				} else if (serialAvailable <= 8) {
					codeType = 26;
				} else {
					codeType = 34;
				}
			}
			if (codeType == 4) {
				// Serial.print(F("Serial read: "));
				Serial.println(code);
			} else if (codeType == 26 || codeType == 34) {
				//recombine codes split into facility code and ID number
				unsigned long top = (code / 100000);
				unsigned long bottom = code % 100000;
				if (top <= 0xFFFF && bottom <= 0xFFFF) {
					code = (top << 16) | bottom;
				} else { //Not valid facility code + ID
					byte i;
					for (i = (ulSize * 8) - 1; !bitRead(code, i) && (i >= 4); --i) {}
					codeType = i + 1;
				}
			}
		}

		switch (mode) {
			case 0:
				normalOperation(codeType, code, now);
				break;
			case 1:
				confSelect(wgAvailable, code);
				break;
			case 2:
				addCode(codeType, code);
				break;
			case 3:
				deleteCode(codeType, code);
				break;
			case 4:
				setConfCode(codeType, code);
				break;
			case 44:
				deleteBySlot(wgAvailable, codeType, code);
				break;
		}
		
		interrupts(); 
	}

	doInit(now); //Detect initialization button

	// Write to output pins
	if ( unlockEnd && (now < unlockEnd) ) {
		digitalWrite(DOOR_PIN, LOW);
	} else { 
		if (unlockEnd) {
			if (SERIAL_ENABLE) { Serial.println(F("DOOR LOCKED")); }
			unlockEnd = 0;
		}
		digitalWrite(DOOR_PIN, HIGH);
	}
	
	if ( (DOORBELL_DURATION && doorBellEnd && (now < doorBellEnd)) || doorbellButton ) {
		digitalWrite(DOORBELL_PIN, LOW);
	} else {
		if(DOORBELL_DURATION && doorBellEnd) {
			doorBellEnd = 0;
			if (SERIAL_ENABLE) {Serial.println(F("DONG!")); }
		}
		digitalWrite(DOORBELL_PIN, HIGH);
	}

	//Set LED overrides for configuration modes
	if (SERIAL_ENABLE && HIDE_CODES == 1 && hideCodes == 0) {
		//Hide Codes temporarily off
		ledTone(0b011, 0b01010101, 255); //amber, 4Hz, forever
	} else if (mode && repeat != 1) {
		switch (mode) {
			case 1: // Configuration mode
				ledTone(0b011, 0b11111111, 255); //amber, steady, forever
				break;
			case 2: // Add
				ledTone(0b010, 0b11001100, 255); //green, 2Hz, forever
				break;
			case 3: // Delete
				ledTone(0b001, 0b11001100, 255); //red, 2Hz, forever
				break;
			case 4: // Set configuration code
				ledTone(0b011, 0b11001100, 255); //amber, 2Hz, forever
				break;
		}
	}

	doLEDTone(now); //Process LED and buzzer patterns
}
//////////////////////////////END OF LOOP///////////////////////////////

void enterConf(byte i, byte m) {
	if (m == 0 || m== 1) {
		m = 1;
		if (SERIAL_ENABLE) {
			Serial.println(F("**CONFIGURATION MODE**"));
			printCodes();
			Serial.println(F("1: Add | 2: Delete | 3: Set Conf Code | 4: Delete #"));
			if (hideCodes) {
				Serial.print(F("0: Unhide codes"));
			} else {
				Serial.print(F("0: Hide codes"));
			}
			Serial.println(F(" | #: Exit"));
		}
	}
	pin = 0;
	pinCount = 0;
	timeoutTime = millis();
	mode = m; // Go to input mode
	ledTone(i, 0b10100, 1); //two flashes, 4 Hz, once
}

void enterNormal(byte i) {
	if (SERIAL_ENABLE) { Serial.println(F("***NORMAL OPERATION***")); }
	mode = 0;
	unlockEnd = 0;
	timeoutTime = 0;
	pin = 0;
	pinCount = 0;
	ledTone(i, 0b1010100, 1); //three flashes, 4 Hz, once
}

/////////////////////////////////MODES//////////////////////////////////

void normalOperation(byte codeType, unsigned long code, unsigned long now) {
	if (codeType == 4) { //keypad input
		if (code == ENTER_KEY) { // (* key)
			pinCount = 0;
			pin = 0;
			code = 0;
			if (DOORBELL_DURATION) { // Trigger doorbell
				doorBellEnd = now + DOORBELL_DURATION;
				ledTone(0b010, 0b1111, 1); //green, 1/2 second, once
				if (SERIAL_ENABLE) {Serial.print(F("DING! ")); }
			}
		} else {
			code = assemblePIN(code);
		}
	} else if (codeType == 8) {
		codeType == 4;
		assemblePIN((code & 0xf0) >> 4);
		code = assemblePIN(code & 0x0f);
	} else if (codeType == 24 || codeType == 32) {
		codeType += 2;
	}
		
	if (SERIAL_ENABLE && code) { 
		Serial.print(F("Code: "));
		formatCode(code, codeType);
		if (hideCodes) {
			Serial.println("");
		} else {
			Serial.print(F(" -- W")); Serial.println(codeType, DEC);
		}
	}
	
	if(code) {
		int foundSlot = codeMatch(code);
		
		if (foundSlot) { //code found in EEPROM
			lockoutDuration = LOCKOUT_DURATION;
			if (foundSlot == ulSize) { // First stored code value enters configuration mode
				enterConf(0b111, 1); //amber beep, mode 1
			} else { // Other stored codes unlock door
				if (SERIAL_ENABLE) { Serial.println(F("DOOR UNLOCKED")); }
				unlockEnd = now + (UNLOCK_DURATION * 1000);
				pinCount = 0;
				pin = 0;
				ledTone(0b010, 0b11111111, UNLOCK_DURATION); //green, 1 second, number of unlocked seconds
			}
		} else {
			if (SERIAL_ENABLE) { Serial.print(F("Code not recognized.")); }
			if (lockoutDuration) {
				if (SERIAL_ENABLE) {
					Serial.print(F(" Lockout for "));
					Serial.println(lockoutDuration);
				}
				lockoutEnd = now + (lockoutDuration * 1000);
				ledTone(0b101, 0b10101010, lockoutDuration); //red beep, 4Hz, until end of lockout
				if ((lockoutMax == 0) || ((lockoutDuration * 2) <= lockoutMax)) {
					lockoutDuration *= 2;
				}
			}
		}
	}
}

void confSelect(bool wgAvailable, unsigned long code) {
	if ((code >= 1 ) && (code <= 3)) {
		timeoutTime = millis();
		mode = code + 1;
		//ledTone(0b111, 0b1, 1); //amber beep, 1/8 second, once
		if (SERIAL_ENABLE) {
			switch (code) {
				case 1:
					Serial.println(F("*******ADD CODE*******"));
					break;
				case 2:
					Serial.println(F("*****DELETE CODE******"));
					printCodes();
					break;
				case 3:
					Serial.println(F("***SET CONFIG CODE****"));
					break;
			}
			Serial.println(F("Enter code or scan tag... | #: Exit"));
		}
	} else if ((SERIAL_ENABLE) && (!wgAvailable) && (code == 4)) { // Only available via serial
		Serial.println(F("***DELETE BY SLOT #***"));
		printCodes();
		Serial.println(F("Enter slot # to delete including leading 0 (type '#' to exit):"));
		timeoutTime = millis();
		mode = 44;
	} else if ((SERIAL_ENABLE) && (!wgAvailable) && (code == 0)) { // Only available via serial
		hideCodes = 1 - hideCodes;
		enterConf(0b111, 1); //amber beep, mode 1
	} else {
		enterNormal(0b111); //amber beep
	}
}

void addCode(byte codeType, unsigned long code) {
	if (codeType == 4 && code != ENTER_KEY) { //keypad input
		code = assemblePIN(code); 
	} else if (codeType == 8) {
		codeType == 4;
		assemblePIN((code & 0xf0) >> 4);
		code = assemblePIN(code & 0x0f);
	}

	if (code && code != ENTER_KEY) { //if there is a code
		if (!codeMatch(code)) { //if the code is new
			int foundSlot = findSlot();
			if (foundSlot) { //if there is an open slot
				EEPROM.put(foundSlot,code);
				if (SERIAL_ENABLE) { 
					Serial.print(F("Code "));
					formatCode(code, codeType);
					Serial.print(F(" stored in slot ")); 
					Serial.println((foundSlot/ulSize) - 1); 
				}
				if (USE_CRC) { EEPROM.put(0,eepromCRC()); }
				enterConf(0b110, 2); //green beep, mode 2
			} else { //if memory is full
				if (SERIAL_ENABLE) { Serial.print(F("Memory full! Delete existing code first."));}
				enterConf(0b101, 1); //red beep, mode 1
			}
		} else { //if the code exists
			if (SERIAL_ENABLE) {
				Serial.print(F("Code "));
				formatCode(code, codeType);
				Serial.println(F(" already exists!"));
			}
			enterConf(0b101, 2); //red beep, mode 2
		}
	}
}

void deleteCode(byte codeType, unsigned long code) {
	if (codeType == 4 && code != ENTER_KEY) { //keypad input
		code = assemblePIN(code); 
	} else if (codeType == 8) {
		codeType == 4;
		assemblePIN((code & 0xf0) >> 4);
		code = assemblePIN(code & 0x0f);
	}
	
		if (code && code != ENTER_KEY) { //if there is a code
			int foundSlot = codeMatch(code);
			
			if (foundSlot && (foundSlot != ulSize)) { //if the code exists and isn't conf code
				EEPROM.put(foundSlot, (unsigned long)(0));
				if (SERIAL_ENABLE) {
					Serial.print(F("Code "));
					formatCode(code, codeType);
					Serial.print(F(" deleted from slot "));
					Serial.println((foundSlot/ulSize) - 1);
				}
				if (USE_CRC) { EEPROM.put(0,eepromCRC()); }
				enterConf(0b110, 3); //green beep, mode 3
			} else { //if the code doesn't exist
				if (SERIAL_ENABLE) {
					Serial.print(F("Code "));
					formatCode(code, codeType);
					Serial.println(F(" not found!"));
				}
				enterConf(0b101, 3); //red beep, mode 3
			}
		}
}

void setConfCode(byte codeType, unsigned long code) {
	if (codeType == 4 && code != ENTER_KEY) { //keypad input
		code = assemblePIN(code); 
	} else if (codeType == 8) {
		codeType == 4;
		assemblePIN((code & 0xf0) >> 4);
		code = assemblePIN(code & 0x0f);
	}
	
	if (code) { //if there is a code
		if (!codeMatch(code)) { //if the code is new
			EEPROM.put(ulSize, code);
			if (SERIAL_ENABLE) {
				Serial.print(F("Configuration code set to "));
				formatCode(code, codeType);
				Serial.println(F("."));
			}
			EEPROM.put(0,eepromCRC());
			enterNormal(0b110); //green beep
		} else { //if the code exists
			if (SERIAL_ENABLE) {
				Serial.print(F("Code "));
				formatCode(code, codeType);
				Serial.println(F(" already in use!"));
			}
			enterNormal(0b101); //red beep
		}
	}
}

void deleteBySlot(bool wgAvailable, byte codeType, unsigned long code) {
	if (!wgAvailable) {
		if (serialAvailable <= 1) {
			code = assemblePIN(code, 2); 
		}
	
		if (code) {
			if (code < ((eLength/ulSize)-1)) {
				int foundSlot = ulSize*(code+1);
				unsigned long readCode;
				EEPROM.get(foundSlot, readCode);
				if (readCode) {
					EEPROM.put(foundSlot, (unsigned long)(0));
					if (SERIAL_ENABLE) {
						Serial.print(F("Code "));
						formatCode(readCode, 0);
						Serial.print(F(" deleted from slot "));
						Serial.println(code);
					}
					if (USE_CRC) { EEPROM.put(0,eepromCRC()); }
					printCodes();
					Serial.println(F("Enter slot # to delete ('#' to exit):"));
					enterConf(0b110, 44); //green beep, mode 3
				} else {
					Serial.print("Invalid slot #: ");
					Serial.println(code);
				}
			} else {
				Serial.print("Invalid slot #: ");
				Serial.println(code);
			}
		}
	} else {
		assemblePIN(ESC_KEY);
	}
}
				
////////////////////////////HELPER FUNCTIONS////////////////////////////

void ledTone(byte i, byte p, byte r) {
	if (r != 255) { //Reset pattern if it's not an infinite loop
		patternPosition = 0b00000001;
		nextPattern = millis() + 125;
	}
	indicators = i;
	pattern = p;
	repeat = r;
}

void doLEDTone(unsigned long now) {
	byte r=HIGH, g=HIGH, b=HIGH;

	if(repeat) {
		if (nextPattern && (now > nextPattern)) {
			if (patternPosition == 0b10000000 || !patternPosition) {
				patternPosition = 0b00000001;
				if (repeat != 255) { repeat--; } // 255 represents infinite loop
			} else {
				patternPosition = patternPosition << 1;
				if ( (repeat == 1) && (patternPosition > pattern) ) { repeat = 0; }
			}
			nextPattern = 0;
		}
	
		if (!repeat) { patternPosition = 0b00000001; }
		if (!nextPattern) { nextPattern = now + 125; }
		
		if (pattern && (pattern & patternPosition)) {
			if (indicators & 0b001) { r = LOW; }
			if (indicators & 0b010) { g = LOW; }
			if (indicators & 0b100) { b = LOW; }
		}
	} else {
		pattern = 0;
		indicators = 0;
	}

	digitalWrite(RED_LED_PIN, r);
	digitalWrite(GREEN_LED_PIN, g);
	digitalWrite(BUZZER_PIN, b);
	digitalWrite(LED_BUILTIN, !b); //Mirror buzzer on on-board LED
}

unsigned long eepromCRC(void) {
	unsigned long salt=((unsigned long)(ID_1) << 24) + ((unsigned long)(ID_2) << 16) + ((unsigned long)(ID_3) << 8);
	if (USE_CRC) {
		//get CRC of EEPROM (except stored CRC at start)
	
		const unsigned long crcTable[16] = {
			0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
			0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
			0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
			0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
		};
	
		unsigned long crc = ~0L;
	
		for (int i = ulSize; i < eLength; ++i) {
			crc = crcTable[(crc ^ EEPROM[i]) & 0x0f] ^ (crc >> 4);
			crc = crcTable[(crc ^ (EEPROM[i] >> 4)) & 0x0f] ^ (crc >> 4);
			crc = ~crc;
		}
		return crc + salt;
	} else {
		// return static ID value
		return salt;
	}
}

int codeMatch(unsigned long code) {
	if (code) {
		for (int i = ulSize; i < eLength; i += ulSize) {
			unsigned long readCode;
			EEPROM.get(i, readCode);
			if (readCode == code) { return i; }
		}
	}
	return 0;
}

void printCodes(void) {
	if (SERIAL_ENABLE) {
		for (int i = ulSize * 2; i < eLength; i += ulSize) {
			unsigned long readCode;
			EEPROM.get(i, readCode);
			if (readCode) {
				if ( (i - 1) / ulSize < 10) { Serial.print(F("0")); }
				Serial.print( (i - 1) / ulSize);
				Serial.print(F(". "));
				formatCode(readCode, 0);
				Serial.println(F(""));
			}
		}
	}
}

int findSlot(void) {
	int lastAddress = eLength - ulSize;
	unsigned long lastCode;
	EEPROM.get(lastAddress, lastCode);

	if(!lastCode) {
		//Code storage hasn't wrapped around
		//Find end of stored codes
		for (int i = lastAddress; i >= ulSize; i -= ulSize){ 
			unsigned long readCode;
			EEPROM.get(i, readCode);
			if (readCode) { return i + ulSize; }
		}
	} else {
		//Code storage has wrapped around
		//Find first blank slot
		for (int i = ulSize * 2; i < lastAddress; i += ulSize){ 
			unsigned long readCode;
			EEPROM.get(i, readCode);
			if (!readCode) { return i; }
		}
	}
	return 0;
}

unsigned long assemblePIN(unsigned long code, byte pinLength=PIN_LENGTH) {
	if (code == ESC_KEY) { // (# key)
		code = 0;
		timeoutTime = 0;
		if (pinCount) {
			if (!mode) {
				enterNormal(0b111); //amber beep
			} else {
				enterConf(0b111, mode); //amber beep
			}
		} else {
			if (!mode) { //no numbers entered in normal mode
				ledTone(0b0, 0b1, 1); //nothing
				if (SERIAL_ENABLE) { Serial.println(F("***NORMAL OPERATION***")); }
			} else if (mode == 1) { //no numbers entered in conf
				enterNormal(0b111); //amber beep
			} else { //no numbers entered in add/delete/change mode
				enterConf(0b111, 1); //amber beep
			}
		}
	} else { // Handle 4-bit PINs
		// ledTone(0b011, 0b1, 1); //amber, 1/8 second, once
		timeoutTime = millis();
		pin = (pin * 10) + code;
		pinCount++;
		if (pinCount >= pinLength) {
			pinCount = 0;
			code = pin;
			pin = 0;
		} else {
			code = 0;
			// if (SERIAL_ENABLE) { Serial.print(F("PIN so far: ")); Serial.println(pin); }
		}
	}
	return code;
}

int formatCode(unsigned long code, int type) {
	if (type == 0) {
		int n = 0;
		for(unsigned long c = code; c > 0; c /= 10) { n++; }
		if ( (code == 0) || (n <= PIN_LENGTH) ) {
			type = 4;
		} else {
			int n = 0;
			for(unsigned long c = code; c > 0; c >>= 1) { n++; }
			type = (n <= 24) ? 26 : 34;
		}
	}
	if (hideCodes) {
		Serial.print(F("type W"));
		Serial.print(type);
	} else {
		char codeStr[11];
		if (type == 26 || type == 34) {
			char formatStr[] = "%0*u%05u";
			formatStr[2] = '0' + (type == 26 ? 3 : 5); //replace * with width
			sprintf(codeStr, formatStr, (unsigned int)(code >> 16), (unsigned int)(code & 0xFFFF));
		} else {
			char formatStr[] = "%0*lu";
			formatStr[2] = '0' + (type == 4 ? PIN_LENGTH : 10); //replace * with width
			sprintf(codeStr, formatStr, code);
		}
		Serial.print(codeStr);
	}
}

void doInit(unsigned long now) {
	if (!digitalRead(INIT_BUTTON_PIN)) {
		if (initEnd) {
			if (now > initEnd) { 
				if (SERIAL_ENABLE) { Serial.println(F("Press RESET button to reinitialize...")); }
				EEPROM.put(0, (unsigned long)(0));
				ledTone(0b010, 0b10101010, 255); //green, 4Hz, forever
			} else if (SERIAL_ENABLE) {
				Serial.print(F("Keep holding for "));
				Serial.print((int)((initEnd - now)/1000));
				Serial.println(F(" seconds to clear EEPROM..."));
			}
		} else {
			mode = 0;
			ledTone(0b111, 0b10101010, 10); //amber beep, 4Hz, 10 seconds
			initEnd = now + 10000;
			lockoutEnd = initEnd;
		}
	} else if (initEnd) {
		ledTone(0b0, 0, 0); //LED off
		initEnd = 0;
		lockoutEnd = 0;
	}
}

/*
Changelog
---------

* 0.1.0 - Initial pre-release
* 0.2.0 - Implement serial control, CRC error checking
   * 0.2.1 - Add pin timeout
* 1.0.0 Added delete by slot #, conf timeout, and doorbell button
   * 1.0.1 - Refine doorbell button behavior
   * 1.0.2 - Refine serial output
   * 1.0.3 - Allow delete by slot to work with 2-digit slot numbers, zero pad codes
* 1.1.0 - "Hide codes in serial output" option added to prevent bluetooth sniffing
   * 1.1.1 - Get rid of timeElapsed() function, format codes as facility+id for display
   * 1.1.2 - Fix rounding errors. Allow disabling lockout, lockout max, timeout, and doorbell.

Copyright
---------
### COPYRIGHT 2025 ZAN HECHT
ARDUINO ACCESS CONTROL is licensed under a
Creative-Commons Attribution Share-Alike License.
http://creativecommons.org/licenses/by-sa/3.0/
You are free:
to Share — to copy, distribute and transmit the work
to Remix — to adapt the work
to make commercial use of the work
Under the following conditions:
* Attribution — You must attribute the work in the manner specified by the
author or licensor (but not in any way that suggests that they endorse you or
your use of the work).
* Share Alike — If you alter, transform, or build upon this work, you may
distribute the resulting work only under the same or similar license to this
one.
With the understanding that:
* Waiver — Any of the above conditions can be waived if you get permission
from the copyright holder.
* Public Domain — Where the work or any of its elements is in the public
domain under applicable law, that status is in no way affected by the license.
* Other Rights — In no way are any of the following rights affected by the
license:
* Your fair dealing or fair use rights, or other applicable copyright
exceptions and limitations;
* The author's moral rights;
* Rights other persons may have either in the work itself or in how the work
is used, such as publicity or privacy rights.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the full text of the Create-Commons license
linked to above for more details.
*/
