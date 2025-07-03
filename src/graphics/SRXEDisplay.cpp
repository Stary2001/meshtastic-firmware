#include "configuration.h"

#ifdef SMART_RESPONSE_XE
#include "SPILock.h"
#include "SRXEDisplay.h"
#include "main.h"

#include "srxe_lcddraw.h"
#include "srxe_lcdtext.h"

#define SCREEN_WIDTH 384
#define LCD_BYTES_PER_LINE 128

// Constructor
SRXEDisplay::SRXEDisplay(uint8_t address, int sda, int scl, OLEDDISPLAY_GEOMETRY geometry, HW_I2C i2cBus)
{
    this->geometry = GEOMETRY_RAWMODE;
    this->displayWidth = 384;
    this->displayHeight = 136;

    this->displayBufferSize = (384 * 136) / 8;

    this->lcdBuffer = (uint8_t *)malloc(128 * 136);
}

// Write the buffer to the display memory
void SRXEDisplay::display(void)
{
    uint16_t x, y;

    memset(lcdBuffer, 0, LCD_BYTES_PER_LINE * LCD_HEIGHT);

    // I hate this, but it's how TFTDisplay does it
    for (y = 0; y < displayHeight; y++) {
        for (x = 0; x < displayWidth; x++) {
            auto isset = buffer[x + (y / 8) * displayWidth] & (1 << (y & 7));
            if (isset) {
                uint8_t pixel = 0;
                // 3 pixels are packed into a byte in the for bbxbbxbb where 'x' is not used and should be 0

                if ((x % 3) == 2) {
                    pixel = 0b11;
                } else if ((x % 3) == 1) {
                    pixel = 0b11 << 3;
                } else {
                    pixel = 0b11 << 6;
                }
                lcdBuffer[y * LCD_BYTES_PER_LINE + (x / 3)] |= pixel;
            }
        }
    }

    _lcd_set_active_area(0, 0, LCD_WIDTH, LCD_HEIGHT);
    for (int i = 0; i < LCD_HEIGHT; i++) {
        _lcd_write_data_block(lcdBuffer + i * LCD_BYTES_PER_LINE, LCD_WIDTH);
    }
    _lcd_end_active_area();
}

// Send a command to the display (low level function)
void SRXEDisplay::sendCommand(uint8_t com)
{
    (void)com;
    // Drop all commands to device (we just update the buffer)
}

void SRXEDisplay::setDetected(uint8_t detected)
{
    (void)detected;
}

// Connect to the display - variant specific
bool SRXEDisplay::connect()
{
    lcdInit();
    return true;
}

#endif
