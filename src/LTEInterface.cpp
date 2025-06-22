#include "LTEInterface.h"
#include "DebugConfiguration.h"

#if HAS_LTE

LTEInterface *lteInterface = nullptr;

LTEInterface::LTEInterface() : concurrency::OSThread("LTEInterface")
{
    lteInterface = this;
}

// adjust LTE modem settings
// AT+CEREG=1
// AT+COPS? -> AT+COPS=0
// specify APN with AT+CGDCONT=1,"IP","web"
// connect with AT+CGACT=1,1?

void LTEInterface::sendCommand(const char *s)
{
    LOG_DEBUG("Modem >>: %s\r\n", s);
    Serial2.write(s);
    Serial2.write("\r\n");
}

bool LTEInterface::tryReadLine()
{
    if (Serial2.available()) {
        size_t len = Serial2.readBytesUntil('\n', lineBuffer, 255);
        if (len == 0)
            return false;

        if (lineBuffer[len - 1] == '\r') {
            lineBuffer[len - 1] = 0;
        }
        lineBuffer[len] = 0;

        LOG_DEBUG("[%i] Modem <<: '%s'\r\n", len, lineBuffer);

        return true;
    } else {
        return false;
    }
}

void LTEInterface::waitReadLine()
{
    while (tryReadLine() == false) {
        delay(100);
    }
}

bool startswith(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

void LTEInterface::handleRadioResponse()
{
    while (tryReadLine()) {
        if (startswith(lineBuffer, "+IPD")) {
            // Got n bytes of data
            int len = atoi(lineBuffer + 4);

            if (connectionBuffers[1].write_pos + len > connectionBuffers[1].len) {
                abort();
            }

            size_t read_len = 0;
            while (read_len < len) {
                read_len +=
                    Serial2.readBytes(connectionBuffers[1].data + connectionBuffers[1].write_pos + read_len, len - read_len);
            }
            connectionBuffers[1].write_pos += len;
        } else if (startswith(lineBuffer, "+NETOPEN")) {
            netopenInitialized = true;
        } else if (startswith(lineBuffer, "+IP ERROR: Network is already opened")) {
            netopenInitialized = true;
        } else if (startswith(lineBuffer, "+CEREG")) {
            // cereg reply: +CEREG: 0,1
            char *sep = strchr(lineBuffer, ',');
            if (sep != nullptr) {
                if (strcmp(sep + 1, "1") == 0 || strcmp(sep + 1, "5") == 0) {
                    registered = true;
                }
            }
        } else if (startswith(lineBuffer, "+CGEV")) {
            // +CGEV: ME PDN ACT 8,0
            // 1234567
            if (startswith(lineBuffer + 7, "EPS PDN ACT")) {
                packetDataReady = true;
            } else if (startswith(lineBuffer + 7, "NW PDN DEACT")) {
                packetDataReady = false;
            }
        } else if (startswith(lineBuffer, "+CIPOPEN")) {

            char *sep = strchr(lineBuffer, ',');
            if (sep != nullptr) {
                *sep = 0;
                if (strcmp(sep + 1, "0") == 0) {
                    connectionState[1] = true;
                    connectionBuffers[1] = Buffer();
                    connectionBuffers[1].data = (char *)malloc(1024);
                    connectionBuffers[1].len = 1024;
                    connectionBuffers[1].read_pos = 0;
                    connectionBuffers[1].write_pos = 0;
                }
            }
        } else if (startswith(lineBuffer, "+CIPSEND")) {
            // just ignore this status reply
        } else if (strcmp(lineBuffer, "OK") == 0) {
            radioAlive = true;
        } else if (strlen(lineBuffer) == 0) {
            // got empty line, ignore
        } else {
            LOG_DEBUG("unk reply '%s'\r\n", lineBuffer);
        }
    }
}

const char *state_to_str(LTEState state)
{
    switch (state) {
    case Start:
        return "Start";
    case WaitForReset:
        return "WaitForReset";
    case Setup:
        return "Setup";
    case WaitForRegister:
        return "WaitForRegister";
    case WaitForData:
        return "WaitForData";
    case WaitForNetopen:
        return "WaitForNetopen";
    case Connect:
        return "Connect";
    case Done:
        return "Done";
    default:
        return "Update your damn code";
    }
}

int32_t LTEInterface::runOnce()
{
    handleRadioResponse();

    if (state != Done) {
        LOG_DEBUG("Modem state: %s\r\n", state_to_str(state));
    }

    int32_t delay = 100;
    switch (state) {
    case Start: {
        Serial2.begin(115200);
        Serial2.setTimeout(500);

        int pwrkey = PA_0;
        int reset = PB_3;

        // generic stm32 variant uses different arduino pin numbers, be careful
        pinMode(pwrkey, OUTPUT);
        pinMode(reset, OUTPUT);

        digitalWrite(pwrkey, 0);
        digitalWrite(reset, 1);

        sendCommand("AT+CRESET");
        waitReadLine();

        handleRadioResponse();
        radioAlive = false;
        registered = false;
        packetDataReady = false;
        netopenInitialized = false;

        for (int i = 0; i < MAX_CONNS; i++) {
            connectionState[1] = false;
        }

        state = WaitForReset;

        delay = 1000;
    }
    case WaitForReset:
        if (radioAlive) {
            state = Setup;
        } else {
            sendCommand("AT");
            delay = 1000;
        }
        break;
    case Setup:
        sendCommand("AT+CIPSRIP=0");
        handleRadioResponse();

        sendCommand("AT+CEREG?");
        handleRadioResponse();

        state = WaitForRegister;
        delay = 1000;
        break;
    case WaitForRegister:
        if (registered) {
            state = WaitForData;
        } else {
            sendCommand("AT+CEREG?");
            handleRadioResponse();
            delay = 1000;
        }
        break;
    case WaitForData:
        if (packetDataReady) {
            sendCommand("AT+NETOPEN");
            handleRadioResponse();
            state = WaitForNetopen;
        }
        break;
    case WaitForNetopen:
        if (netopenInitialized) {
            sendCommand("AT+CIPOPEN?");
            state = Connect;
        }
        break;
    case Connect:
        // LTEClient *client = new LTEClient();
        // client->connect("9net.org", 80);
        state = Done;
        break;
    }

    return delay;
}

bool LTEInterface::connected()
{
    return netopenInitialized;
}

LTEClient::LTEClient() : Client() {}

int LTEClient::connect(IPAddress ip, uint16_t port)
{
    String s = ip.toString();
    Serial2.printf("AT+CIPOPEN=1,\"TCP\",\"%s\",%i\r\n", s.c_str(), port);

    while (true) {
        lteInterface->handleRadioResponse();
        if (lteInterface->connectionState[1] == true) {
            break;
        }
        delay(100);
    }
    return 1;
}

int LTEClient::connect(const char *host, uint16_t port)
{
    Serial2.printf("AT+CIPOPEN=1,\"TCP\",\"%s\",%i\r\n", host, port);

    while (true) {
        lteInterface->handleRadioResponse();
        if (lteInterface->connectionState[1] == true) {
            break;
        }
        delay(100);
    }

    return 1;
}

size_t LTEClient::write(uint8_t c)
{
    Serial2.write("AT+CIPSEND=1,1\r\n");
    while (true) {
        if (Serial2.available()) {
            if (Serial2.read() == '>') {
                break;
            }
        }
    }
    Serial2.write(c);

    while (true) {
        if (Serial2.available()) {
            Serial2.read();
            break;
        }
    }

    return 1;
}

size_t LTEClient::write(const uint8_t *buf, size_t size)
{
    Serial2.printf("AT+CIPSEND=1,%u\r\n", size);
    while (true) {
        if (Serial2.available()) {
            if (Serial2.read() == '>') {
                break;
            }
        }
    }
    Serial2.write(buf, size);
    size_t expected = size;
    while (expected) {
        if (Serial2.available()) {
            Serial2.read();
            expected--;
        }
    }
    lteInterface->handleRadioResponse();

    return size;
}
int LTEClient::available()
{
    lteInterface->handleRadioResponse();
    return lteInterface->connectionBuffers[1].write_pos - lteInterface->connectionBuffers[1].read_pos;
}
int LTEClient::read()
{
    size_t pos = lteInterface->connectionBuffers[1].read_pos++;

    if (lteInterface->connectionBuffers[1].read_pos == lteInterface->connectionBuffers[1].write_pos) {
        lteInterface->connectionBuffers[1].read_pos = 0;
        lteInterface->connectionBuffers[1].write_pos = 0;
    }
    return lteInterface->connectionBuffers[1].data[pos];
}
int LTEClient::read(uint8_t *buf, size_t size)
{
    size_t pos = lteInterface->connectionBuffers[1].read_pos;
    lteInterface->connectionBuffers[1].read_pos += size;

    if (lteInterface->connectionBuffers[1].read_pos == lteInterface->connectionBuffers[1].write_pos) {
        lteInterface->connectionBuffers[1].read_pos = 0;
        lteInterface->connectionBuffers[1].write_pos = 0;
    }

    memcpy(buf, lteInterface->connectionBuffers[1].data + pos, size);
    return size;
}
int LTEClient::peek()
{
    return lteInterface->connectionBuffers[1].data[lteInterface->connectionBuffers[1].read_pos];
}
void LTEClient::flush() {}
void LTEClient::stop() {}

uint8_t LTEClient::connected()
{
    return lteInterface->connectionState[1];
}

LTEClient::operator bool()
{
    return true;
}

IPAddress LTEClient::remoteIP()
{
    return IPAddress();
}
#endif