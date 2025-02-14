## ACCESS CONTROL v1.1.2
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
