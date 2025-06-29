/* ************************************************************************************
 * File:    common.h
 * Date:    2021.08.12
 * Author:  Bradan Lane Studio
 *
 * This content may be redistributed and/or modified as outlined under the MIT License
 *
 * ************************************************************************************/

/* ---

## common.h - common helper functions used by multiple modules of the SRXEcore

The SRXE core has been derived from the work of multiple sources including:
 - Original keyboard and LCD code - BitBank Software, Inc. / Larry Bank bitbank@pobox.com
 - Updates to keyboard mapping and LCD scroll code - fdufnews fdufnews@free.fr
 - Original RF code SparkFun Electronics / Jim Lindblom

The code has been extensively refactored.
 - The font generation is all new to support more flexibility in font choices.
 - The text drawing is all new to match the new font generation.
 - The bitmap handling has been updated to handle bitmaps of any width * height up to that of the LCD.
 - High level UI drawing operations have been added.
 - Debugging capabilities have been added (using the 10-pins accessed from the battery compartment):
        - BitBang UART (9600-8N1) using the exposed TDI pin
    - LEDs may be connected to TDK, TMS, TDO, and TDI

_The debugging capabilities are generally available and have been tested with the **Enigma Development Adapter**._

--------------------------------------------------------------------------
--- */

#ifndef __SRXE_COMMON_
#define __SRXE_COMMON_

#define srxePinMode pinMode
#define srxeDigitalRead digitalRead
#define srxeDigitalWrite digitalWrite

/* ---
#### long srxeMap(long val, long in_min, long in_max, long out_min, long out_max)

Re-map a number from one range to another.

The return value is the input value mapped using the min/max mapping from in to out.
--- */
long srxeMap(long val, long in_min, long in_max, long out_min, long out_max)
{
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*

### COMMON SPI HANDLING

The SRXE uses SPI to communicate with both the LCD controller and the FLASH chip on the PCB.

Each SPI device needs an `SS` _select_ pin. By default, this pin is `HIGH`. It is pulled `LOW` to activate the device.
after the SPI operation(s) have been performed, the `SS` is pulled `HIGH` again so the SPI interface is available for any other
attached device.
*/

// the CS is uses so multiple devices may share the SPI - Set CS to HIGH so a connected chip will be "deselected" by default

// #define SPI_CS		(SRXE_PORTB | PIN0)
#define SPI_SCK 20
#define SPI_MOSI 22
// #define SPI_MISO	(SRXE_PORTB | PIN3)

#define LCD_CS 6
#define LCD_DC 8
#define LCD_RESET 17

#endif // __SRXE_COMMON_
