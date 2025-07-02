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

#endif // __SRXE_COMMON_
