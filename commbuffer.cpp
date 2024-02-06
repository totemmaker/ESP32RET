#include "commbuffer.h"
#include "Logger.h"
#include "gvret_comm.h"

CommBuffer::CommBuffer()
{
    transmitBufferLength = 0;
}

size_t CommBuffer::numAvailableBytes()
{
    return transmitBufferLength;
}

void CommBuffer::clearBufferedBytes()
{
    transmitBufferLength = 0;
}

uint8_t* CommBuffer::getBufferedBytes()
{
    return transmitBuffer;
}

//a bit faster version that blasts through the copy more efficiently.
void CommBuffer::sendBytesToBuffer(uint8_t *bytes, size_t length)
{
    memcpy(&transmitBuffer[transmitBufferLength], bytes, length);
    transmitBufferLength += length;
}

void CommBuffer::sendByteToBuffer(uint8_t byt)
{
    transmitBuffer[transmitBufferLength++] = byt;
}

void CommBuffer::sendString(String str)
{
    char buff[300];
    str.toCharArray(buff, 300);
    sendCharString(buff);
}

void CommBuffer::sendCharString(char *str)
{
    char *p = str;
    int i = 0;
    while (*p)
    {
        sendByteToBuffer(*p++);
        i++;
    }
    Logger::debug("Queued %i bytes", i);
}

void CommBuffer::sendFrameToBuffer(CAN_FRAME &frame, int whichBus)
{
    uint8_t temp;
    size_t writtenBytes;
    if (settings.useBinarySerialComm) {
        if (frame.extended) frame.id |= 1 << 31;
        transmitBuffer[transmitBufferLength++] = 0xF1;
        transmitBuffer[transmitBufferLength++] = 0; //0 = canbus frame sending
        uint32_t now = micros();
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now & 0xFF);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 8);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 16);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 24);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id & 0xFF);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 8);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 16);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 24);
        transmitBuffer[transmitBufferLength++] = frame.length + (uint8_t)(whichBus << 4);
        for (int c = 0; c < frame.length; c++) {
            transmitBuffer[transmitBufferLength++] = frame.data.uint8[c];
        }
        //temp = checksumCalc(buff, 11 + frame.length);
        temp = 0;
        transmitBuffer[transmitBufferLength++] = temp;
        //Serial.write(buff, 12 + frame.length);
    } else {
        writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], "%ld - %x", micros(), frame.id);
        transmitBufferLength += writtenBytes;
        if (frame.extended) sprintf((char *)&transmitBuffer[transmitBufferLength], " X ");
        else sprintf((char *)&transmitBuffer[transmitBufferLength], " S ");
        transmitBufferLength += 3;
        writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], "%i %i", whichBus, frame.length);
        transmitBufferLength += writtenBytes;
        for (int c = 0; c < frame.length; c++) {
            writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], " %x", frame.data.uint8[c]);
            transmitBufferLength += writtenBytes;
        }
        sprintf((char *)&transmitBuffer[transmitBufferLength], "\r\n");
        transmitBufferLength += 2;
    }
}

void CommBuffer::sendFrameToBuffer(CAN_FRAME_FD &frame, int whichBus)
{
    uint8_t temp;
    size_t writtenBytes;
    if (settings.useBinarySerialComm) {
        if (frame.extended) frame.id |= 1 << 31;
        transmitBuffer[transmitBufferLength++] = 0xF1;
        transmitBuffer[transmitBufferLength++] = PROTO_BUILD_FD_FRAME;
        uint32_t now = micros();
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now & 0xFF);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 8);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 16);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(now >> 24);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id & 0xFF);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 8);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 16);
        transmitBuffer[transmitBufferLength++] = (uint8_t)(frame.id >> 24);
        transmitBuffer[transmitBufferLength++] = frame.length;
        transmitBuffer[transmitBufferLength++] = (uint8_t)(whichBus);
        for (int c = 0; c < frame.length; c++) {
            transmitBuffer[transmitBufferLength++] = frame.data.uint8[c];
        }
        //temp = checksumCalc(buff, 11 + frame.length);
        temp = 0;
        transmitBuffer[transmitBufferLength++] = temp;
        //Serial.write(buff, 12 + frame.length);
    } else {
        writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], "%ld - %x", micros(), frame.id);
        transmitBufferLength += writtenBytes;
        if (frame.extended) sprintf((char *)&transmitBuffer[transmitBufferLength], " X ");
        else sprintf((char *)&transmitBuffer[transmitBufferLength], " S ");
        transmitBufferLength += 3;
        writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], "%i %i", whichBus, frame.length);
        transmitBufferLength += writtenBytes;
        for (int c = 0; c < frame.length; c++) {
            writtenBytes = sprintf((char *)&transmitBuffer[transmitBufferLength], " %x", frame.data.uint8[c]);
            transmitBufferLength += writtenBytes;
        }
        sprintf((char *)&transmitBuffer[transmitBufferLength], "\r\n");
        transmitBufferLength += 2;
    }
}

