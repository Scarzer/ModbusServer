//
// Created by irvy on 8/6/15.
//

#ifndef MODBUSSERVER_MODBUSSERVER_H
#define MODBUSSERVER_MODBUSSERVER_H

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <mutex>
#include <modbus/modbus.h>

class modbusServer {

public:
    modbusServer();
    ~modbusServer();

    void        recieveMessages();

    bool        initModbus(int port, bool debugging);
    bool        isRunning();

    bool        setRegisterValue(int registerNumber, uint16_t value);
    uint16_t    getRegisterValue(int registerNumber);


private:
    modbus_t            *ctx;
    modbus_mapping_t    *mapping;

    bool        m_initialized;
    int         m_modbusSocket;
    int         m_port;
    int         m_errCount;

    // Mapping!
    int         m_numBits;
    int         m_numInputBits;
    int         m_numRegisters;
    int         m_numInputRegisters;
};


#endif //MODBUSSERVER_MODBUSSERVER_H
