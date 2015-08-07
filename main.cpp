#include <iostream>
#include "thread"
#include "modbusServer.h"

using namespace std;


void modbusRunner(modbusServer *server){

    server->initModbus(1502, true);
    server->recieveMessages();
}


modbusServer modSer;

int main() {
    std::thread modSerThread(modbusRunner, &modSer);
    modSerThread.join();


    std::cout << "Running? " << modSer.isRunning() << std::endl;

    return 0;
}