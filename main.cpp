#include <iostream>
#include "thread"
#include "modbusServer.h"

using namespace std;


void modbusRunner(modbusServer *server){
    server->recieveMessages();
}


int main() {
    modbusServer modSer;
    std::thread modSerThread(modbusRunner, &modSer);

    modSer.initModbus(1502,false);

    modSerThread.detach();

    std::cout << "Running? " << modSer.isRunning() << std::endl;
    while(modSer.isRunning() == 1) {
        // Loop through the registers
        for(int i = 0; i < 25; i++){
            std::cout << "Register " << i << ": \t" << modSer.getRegisterValue(i) << "\n";
        }

        sleep(1);
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
    }
    std::cout << "Running? " << modSer.isRunning() << std::endl;

    return 0;
}