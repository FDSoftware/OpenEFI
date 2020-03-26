#include "../variables.h"
#include <string.h>
#include <libopencm3/cm3/scb.h>

#include "commands.h"
#include "../defines.h"
#include "../variables.h"
//Structs para formatear data:
#include "./dataStruct/status.h"
#include "../helpers/bootloader.c"
//Variables de todo el socotroco:
struct serialAPI{
    char buffer[128];
    int dataSize;
} mySerial = {
    {}, 0
};

// Definición de mensajes
typedef struct {
    // Versión del protocolo. Por ahora PROTOCOL_VERSION_1.
    uint8_t protocolVersion;
    // Comando a ejecutar.
    uint8_t command;
    uint8_t subcommand;
    char payload[123];
    // CRC16 del mensaje
    uint16_t checksum;
} SerialMessage;


// Declaracion de funciones:

bool get_frame(char*, int);
char *get_msg(void);
void clear_msg(void);
int get_data_size(void);
void send_message(usbd_device*, SerialMessage*);
void process_frame(usbd_device*, SerialMessage*);
uint16_t crc16(const unsigned char*, uint8_t);


/** Procesa comandos.
 * @param usbd_dev
 * @param message Mensaje recibido.
 */ 
void process_frame(usbd_device* usbd_dev, SerialMessage* message){
    SerialMessage response = {PROTOCOL_VERSION_1, 0, 0, {},0};
    response.protocolVersion = PROTOCOL_VERSION_1;
    response.command = message->command;
    uint16_t localCRC = crc16((unsigned char *) message, 126);
    if (localCRC != message->checksum)
    {
        response.command = COMMAND_ERR;
        response.subcommand = ERROR_INVALID_CHECKSUM;
        send_message(usbd_dev, &response);
        return;
    }
    //Labels de los Structs para que no rompan los switch:
    Status _status = {{}, 0, 0, 0};

    switch (message->protocolVersion){
    case PROTOCOL_VERSION_1:
        switch (message->command)
        {
        case COMMAND_PING:
            memcpy(response.payload, message->payload, sizeof(response.payload));
            break;
        case COMMAND_HELLO:
            response.payload[0] = OPENEFI_VER_MAJOR;
            response.payload[1] = OPENEFI_VER_MINOR;
            response.payload[2] = OPENEFI_VER_REV;
            break;
        case COMMAND_STATUS:
            switch (message->subcommand)
            {
            case STATUS_TMP:
                _status.RPM = _RPM; _status.TEMP = _TEMP; _status.V00 = _V00;
                memcpy(response.payload, &_status, 124);
                break;
            case STATUS_RPM:
                memcpy(response.payload, &_RPM, sizeof(int));
                break;
            default:
                response.command = COMMAND_ERR;
                response.subcommand = ERROR_INVALID_COMMAND;
                break;
            }
            break;
        case COMMAND_BOOTL_SW:
            rtc_backup_write(0, 0x544F4F42UL);
            scb_reset_system();
            return;
        default:
            response.command = COMMAND_ERR;
            response.subcommand = ERROR_INVALID_COMMAND;
        }
        break;

    default:
        // Protocolo invalido.
        response.command = COMMAND_ERR;
        response.subcommand = ERROR_INVALID_PROTOCOL;
    }

    send_message(usbd_dev, &response);
}
/** envia mensajes
 * @param usbd_dev puntero a constructor del USB
 * @param message Mensaje a enviar.
 */
void send_message(usbd_device *usbd_dev, SerialMessage* message){
    message->checksum = crc16((unsigned char *)message, 126);
    char* msg = (char*) message;
    usbd_ep_write_packet(usbd_dev, 0x82, msg, 64);
    for (int i = 0; i < 0x8000; i++) __asm__("nop");
    usbd_ep_write_packet(usbd_dev, 0x82, (msg + 64), 64);
}

bool get_frame(char *tempbuf, int len){
    if ((mySerial.dataSize + len) <= 128){
        memcpy(mySerial.buffer + mySerial.dataSize, tempbuf, len);
        mySerial.dataSize += len;
        if(mySerial.dataSize >= 128){
            return true;
        }
    }
    return false;
}

char *get_msg(){
    return mySerial.buffer;
}

void clear_msg(){
    memset(mySerial.buffer, 0, mySerial.dataSize);
    mySerial.dataSize = 0;
}

int get_data_size(){
    return mySerial.dataSize;
}

uint16_t crc16(const unsigned char* data_p, uint8_t length){
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
    }
    return crc;
}