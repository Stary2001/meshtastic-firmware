#pragma once

#if HAS_LTE
#include "concurrency/OSThread.h"

#include <Client.h>

enum LTEState { Start, WaitForReset, Setup, WaitForRegister, WaitForData, WaitForNetopen, Connect, Done };

#define MAX_CONNS 10

class Buffer
{
  public:
    char *data;
    size_t len;
    size_t read_pos;
    size_t write_pos;
};

class LTEInterface : public concurrency::OSThread
{
  public:
    LTEInterface();

    int32_t runOnce() override;
    bool tryReadLine();
    void waitReadLine();

    void handleRadioResponse();

    void sendCommand(const char *s);

    bool connected();

    char lineBuffer[256];

    LTEState state = Start;
    bool radioAlive = false;
    bool registered = false;
    bool packetDataReady = false;
    bool netopenInitialized = false;

    bool connectionState[MAX_CONNS];
    Buffer connectionBuffers[MAX_CONNS];
};

class LTEClient : public Client
{
  public:
    LTEClient();
    virtual int connect(IPAddress ip, uint16_t port);
    virtual int connect(const char *host, uint16_t port);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buf, size_t size);
    virtual int available();
    virtual int read();
    virtual int read(uint8_t *buf, size_t size);
    virtual int peek();
    virtual void flush();
    virtual void stop();
    virtual uint8_t connected();
    virtual operator bool();

    virtual IPAddress remoteIP();
};

extern LTEInterface *lteInterface;
#endif