//
// Created by irvy on 8/6/15.
//

#include <modbus.h>
#include <error.h>
#include "modbusServer.h"

modbusServer::modbusServer() {
    m_initialized   = false;
    mapping         = modbus_mapping_new(m_numBits, m_numInputBits, m_numInputRegisters, m_numInputRegisters);
}

modbusServer::~modbusServer() {
    modbus_mapping_free(mapping);
    modbus_free(ctx);
}

bool modbusServer::isRunning() {
    return m_initialized;
}

bool modbusServer::initModbus(int port = 502, bool debugging = false) {
    ctx = modbus_new_tcp("0.0.0.0", port);

    modbus_set_debug(ctx, debugging);

    if(ctx == NULL){
        std::cerr << "There was an error allocating the modbus" << std::endl;
        throw -1;
    }
    m_modbusSocket  = modbus_tcp_listen(ctx, 1);

    m_initialized   = true;

}

bool modbusServer::setRegisterValue(int registerNumber, uint16_t value) {
    if(registerNumber > m_numRegisters)
        return false;

    mapping->tab_registers[registerNumber]  = value;
    return true;
}

uint16_t modbusServer::getRegisterValue(int registerNumber) {
    if(!m_initialized) return -1;

    std::mutex mappingLock;

    std::lock_guard<std::mutex> lock(mappingLock);

    uint16_t registerVal = mapping->tab_registers[registerNumber];

    return registerVal;
}

void modbusServer::recieveMessages(){
    modbus_tcp_accept(ctx, &m_modbusSocket);

    if(m_modbusSocket == -1) std::cerr << modbus_strerror(errno) << std::endl;

    int rc;
    uint8_t *query;

    for(;;){
        rc  = modbus_receive(ctx, query);

        if( rc == 0){
            m_errCount  = 0;
            continue;
        } else if (rc > 0){
            m_errCount  = 0;

            modbus_reply(ctx, query, sizeof(query), mapping);

        } else {
            modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK);
            modbus_close(ctx);
            modbus_tcp_accept(ctx, &m_modbusSocket);
            modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_NONE);
            m_errCount++;
        }
        if(m_errCount > 5) {
            m_initialized   = false;
            break;
        }
    }
}