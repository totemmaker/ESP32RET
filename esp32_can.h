#pragma once

class CAN_FRAME {
public:
    union {
        uint8_t bytes[8];
        uint8_t byte[8];
        uint8_t uint8[8];
    } data;
    uint32_t id;
    uint32_t fid;
    uint32_t timestamp;
    uint8_t rtr;
    uint8_t priority;
    uint8_t extended;
    uint8_t length;
};

class CAN_FRAME_FD : public CAN_FRAME { };

class CAN_COMMON {
    int baudrate = 500;
    bool modeListen = false;
    bool beginReady = false;
public:
    uint32_t begin(uint32_t baudrate, uint8_t enPin) { this->baudrate = baudrate; beginReady = true; return 0;}
    // uint32_t begin(uint32_t baudrate) 
    int setRXFilter(uint8_t mailbox, uint32_t id, uint32_t mask, bool extended) {
        if (extended) CAN.setFilterExt(mailbox, id, mask);
        else return CAN.setFilterStd(mailbox, id, mask);
        return 0;
    }
    void setListenOnlyMode(bool state) { this->modeListen = state; }
    int watchFor() {
        if (!beginReady) return 0;
        if (modeListen) CAN.beginListen(baudrate);
        else CAN.begin(baudrate);
        beginReady = false;
        return 0;
    }
	void disable() { CAN.end(); beginReady = false; }
    uint16_t available() { return CAN.readPacket() ? 1 : 0; }
    uint32_t read(CAN_FRAME &msg) { 
        CANPacket packet = CAN.getPacket();
        msg.extended = packet.ext;
        msg.rtr = packet.rtr;
        msg.id = packet.id;
        msg.length = packet.len;
        memcpy(msg.data.bytes, packet.data, packet.len);
        return 1;
    }
    bool sendFrame(CAN_FRAME& txFrame) {
        return CAN.writePacket(txFrame.extended, txFrame.id, txFrame.data.bytes, txFrame.length, txFrame.rtr);
    }

    bool supportsFDMode() { return false; }

    void setDebuggingMode(bool mode) { }
    
    uint32_t beginFD(uint32_t nominalBaudRate, uint32_t fastBaudRate) { return 0; }
    void setCANPins(gpio_num_t rxPin, gpio_num_t txPin) { }
    uint32_t readFD(CAN_FRAME_FD &msg) { return 0; }
    bool sendFrameFD(CAN_FRAME_FD& txFrame) { return false; }
    void enable() { }
};

extern CAN_COMMON CAN0;

#define CAN1 CAN0