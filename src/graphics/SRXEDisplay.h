#pragma once

#ifdef SMART_RESPONSE_XE

#include <OLEDDisplay.h>

class SRXEDisplay : public OLEDDisplay
{
  public:
    SRXEDisplay(uint8_t, int, int, OLEDDISPLAY_GEOMETRY, HW_I2C);

    // Write the buffer to the display memory
    virtual void display();

    // Functions for changing display brightness
    void setDisplayBrightness(uint8_t);

    /**
     * shim to make the abstraction happy
     *
     */
    void setDetected(uint8_t detected);

  protected:
    // the header size of the buffer used, e.g. for the SPI command header
    virtual int getBufferOffset(void) override { return 0; }

    // Send a command to the display (low level function)
    virtual void sendCommand(uint8_t com) override;

    // Connect to the display
    virtual bool connect() override;

  private:
    uint8_t *lcdBuffer;
};

#endif
