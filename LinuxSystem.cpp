#include "SystemComponent.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <ifaddrs.h>
#include <cctype>
#include <netdb.h>
#include <sys/utsname.h>
#include <ifaddrs.h>
#include <sys/utsname.h>

using namespace std;

SystemComponent::SystemComponent(const string& componentName) : name(componentName) {}

const string& SystemComponent::getName() const {
    return name;
}

string CPU::getModelName(){
    ifstream cpuinfo("/proc/cpuinfo");
    string line;
    while (getline(cpuinfo,line)){
        if (line.find("model name") != std::string::npos) {
            return line.substr(line.find(":") + 2);
        }
    }
    return "Unknown CPU Model";
}

double CPU::getCPUInfo(){
    ifstream stat("/proc/stat");
    string cpu;
    stat >> cpu;
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    stat >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    unsigned long long total = user + nice + system + idle + iowait + irq + softirq + steal;
    sleep(1);
    stat.clear();
    stat.seekg(0);
    stat >> cpu;
    stat >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    unsigned long long total2 = user + nice + system + idle + iowait + irq + softirq + steal;

    return (double)(total2 - total) / total2 * 100;
}

CPU::CPU(const string& Cpu):
SystemComponent(Cpu),
modelName(""),
usage(0.0)
{
    modelName = getModelName();
    usage = getCPUInfo();
}

string CPU::getModel() const{
    return modelName;
}

double CPU::getUsage() const{
    return usage;
}


void CPU::showStatus(){
    cout << endl;
    cout << getName() << endl;
    cout << "Model: " << modelName << endl;
    cout << "CPU load: " << usage << "%" << endl;
}

Memory::Memory(const string& Mem) : SystemComponent(Mem), total(0.0), usage(0.0), free(0.0) {
        getMemInfo();
        usage = getUsageInfo();
        free = total - usage;
    }

string Memory::getMemInfo() {
        ifstream meminfo("/proc/meminfo");
        string line;
        string memInfo;


    while (getline(meminfo, line)) {
            if (line.find("MemTotal") != string::npos) {
                istringstream iss(line);
                string key, value;
                iss >> key >> value;
                total = stod(value) / 1024 / 1024; 
            } 
            else if (line.find("MemFree") != string::npos) {
            istringstream iss(line);
             string key, value;
            iss >> key >> value;
            free = stod(value) / 1024 / 1024; 
        }
    }
    return memInfo;
}

double Memory::getUsageInfo() {
    usage = (total - free);
    return usage;
}

double Memory::getTotal() const { return total; }
double Memory::getUsage() const { return usage; }
double Memory::getFree() const { return free; }

void Memory::showStatus() {
    cout << endl;
    cout << getName() << endl;
    cout << "Total Size: " << setprecision(2) << getTotal() << " Gigabyte" << endl;
    cout << "Used Size: " <<  setprecision(2) << getUsage() << " Gigabyte" << endl;
    cout << "Free Size: " << setprecision(2) <<  getFree() << " Gigabyte" << endl;
}
void Disk::getUsageInfo(){
    cout << "Disk";
    ifstream partitions("/proc/partitions");
    string line;
    while (getline(partitions, line)) {
        istringstream iss(line);
        string major, minor, blocks, device;
        iss >> major >> minor >> blocks >> device;

        if (device != "name") {
            try {
                long long blocks_long = stoll(blocks);

                double size_gb = blocks_long * 512.0 / 1024.0 / 1024.0 / 1024.0;

                cout << "Disk: " << device << endl;
                cout << "  Size: " << fixed << setprecision(2) << size_gb << " ГБ" << endl;
                cout << "---------------------" << endl;
            } catch (const std::invalid_argument& e) {
                cerr << blocks << endl;
            }
        }
    }
}

Disk::Disk(const string& Disk) :
SystemComponent(Disk),
DiskName("")
{
    getUsageInfo();
}

double Disk::getTotal(){
    return total;
}

string Disk::getDisk() const {
    return DiskName;
}

void Disk::showStatus(){
    cout << getName() << endl;
}

Net::Net(const string& Net):
SystemComponent(Net),
Hostname(""),
interface("")
{}

string Net::getHost() const{
   struct utsname buf;
    if (uname(&buf) == 0){
        cout << "hostname: " << buf.nodename << endl;
    } else{
        cerr << "get hostname error" << endl;
        return "close";
    }
    return "";
}

string Net::getInterface() const{
    string command = "ifconfig";
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) {
        cerr << "command execute error: " << command << endl;
        return "close"; 
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        cout << buffer; 
    }

    pclose(pipe); 
    return "";
}

void Net::showStatus(){
    cout << endl;
    cout << getName() << endl;
    cout << getHost() << endl;
    cout << getInterface() << endl;
}

int main(){
    CPU cpu("cpu");
    Memory ram("ram");
    Disk disk("disk");
    Net net("net");
    cpu.showStatus();
    ram.showStatus();
    net.showStatus();
}


