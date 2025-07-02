#include "InputBroker.h"
#include "configuration.h"
#include <Wire.h>

class SRXEKeyboard
{
  public:
    enum KeyState { StateIdle = 0, StatePress, StateLongPress, StateRelease };

    struct KeyEvent {
        char key;
        KeyState state;
    };

    SRXEKeyboard();

    void begin(uint8_t addr = SRXE_EXPANDER_ADDR, TwoWire *wire = &Wire);
    void reset(void);

    bool getEvent(InputEvent *e);

  private:
    TwoWire *m_wire;
    uint8_t m_addr;
};
