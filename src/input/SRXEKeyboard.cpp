#include <Arduino.h>

#include "SRXEKeyboard.h"
#include "srxe_keyboard.h"
#include <stdint.h>

#define _REG_VER 1

SRXEKeyboard::SRXEKeyboard() : m_wire(nullptr), m_addr(0) {}

void SRXEKeyboard::begin(uint8_t addr, TwoWire *wire)
{
    m_addr = addr;
    m_wire = wire;

    m_wire->begin();

    reset();
}

void SRXEKeyboard::reset() {}

bool SRXEKeyboard::getEvent(InputEvent *e)
{
    m_wire->requestFrom(m_addr, 1);
    uint8_t key = Wire.read();

    if (key != 0)
        LOG_DEBUG("got %02x\n", key);

    switch (key) {
    case KEY_LEFT:
        e->inputEvent = INPUT_BROKER_LEFT;
        break;
    case KEY_RIGHT:
        e->inputEvent = INPUT_BROKER_RIGHT;
        break;
    case KEY_UP:
        e->inputEvent = INPUT_BROKER_UP;
        break;
    case KEY_DOWN:
        e->inputEvent = INPUT_BROKER_DOWN;
        break;
    case KEY_DEL:
        e->inputEvent = INPUT_BROKER_BACK;
        break;
    case KEY_ESC:
        e->inputEvent = INPUT_BROKER_CANCEL;
        break;
    default:
        if (key < 32 || key > 127) {
            e->inputEvent = INPUT_BROKER_NONE;
        } else {
            e->inputEvent = INPUT_BROKER_ANYKEY;
            e->kbchar = key;
        }
        break;
    }

    return e->inputEvent != INPUT_BROKER_NONE;
}