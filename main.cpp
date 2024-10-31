#include <iostream>
#include "SystemComponent.h" 
using namespace std;

int main() {
    CPU cpu("CPU");
    Memory memory("Memory");
    Disk disk("Disk");
    Net net("Net");

    cpu.showStatus();
    cout << endl;
    memory.showStatus();
    cout << endl;
    net.showStatus();
    cout << endl;

    return 0;
}