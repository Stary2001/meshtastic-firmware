#include "RTC.h"
#include "configuration.h"
#include <stm32wle5xx.h>
#include <stm32wlxx_hal.h>
#include <stdarg.h>

void setBluetoothEnable(bool enable) {}

void playStartMelody() {}

void updateBatteryLevel(uint8_t level) {}

void getMacAddr(uint8_t *dmac)
{
    // https://flit.github.io/2020/06/06/mcu-unique-id-survey.html
    const uint32_t uid0 = HAL_GetUIDw0(); // X/Y coordinate on wafer
    const uint32_t uid1 = HAL_GetUIDw1(); // [31:8] Lot number (23:0), [7:0] Wafer number
    const uint32_t uid2 = HAL_GetUIDw2(); // Lot number (55:24)

    // Need to go from 96-bit to 48-bit unique ID
    dmac[5] = (uint8_t)uid0;
    dmac[4] = (uint8_t)(uid0 >> 16);
    dmac[3] = (uint8_t)uid1;
    dmac[2] = (uint8_t)(uid1 >> 8);
    dmac[1] = (uint8_t)uid2;
    dmac[0] = (uint8_t)(uid2 >> 8);
}

void cpuDeepSleep(uint32_t msecToWake) {}

// Stolen from https://interrupt.memfault.com/blog/cortex-m-hardfault-debug
typedef struct __attribute__((packed)) ContextStateFrame {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t return_address;
    uint32_t xpsr;
} sContextStateFrame;

// NOTE: If you are using CMSIS, the registers can also be
// accessed through CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk
#define HALT_IF_DEBUGGING()                              \
  do {                                                   \
    if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) { \
      __asm("bkpt 1");                                   \
    }                                                    \
} while (0)

char hardfault_message_buffer[256];

void debug_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  size_t length = vsprintf(hardfault_message_buffer, format, args);
  uart_debug_write((uint8_t*)hardfault_message_buffer, length);
  va_end(args);
}

// Disable optimizations for this function so "frame" argument
// does not get optimized away
extern "C" __attribute__((optimize("O0"))) void HardFault_Handler_C(sContextStateFrame *frame) {
   // Try directly using srcwrapper's debug UART function.
   // This must be non-const because uart_debug_write doesn't take a const ptr.
   uint32_t r0;
   uint32_t r1;
   uint32_t r2;
   uint32_t r3;
   uint32_t r12;
   uint32_t lr;
   uint32_t return_address;
   uint32_t xpsr;
   debug_printf("HardFault!\r\n");
   debug_printf("r0: %08x\r\n", frame->r0);
   debug_printf("r1: %08x\r\n", frame->r1);
   debug_printf("r2: %08x\r\n", frame->r2);
   debug_printf("r3: %08x\r\n", frame->r3);
   debug_printf("r12: %08x\r\n", frame->r12);
   debug_printf("lr: %08x\r\n", frame->lr);
   debug_printf("pc[return address]: %08x\r\n", frame->return_address);
   debug_printf("xpsr: %08x\r\n", frame->xpsr);

   HALT_IF_DEBUGGING();

   while(1); 
}
