#ifndef SYSTEMCOMPONENT_H
#define SYSTEMCOMPONENT_H
#include <string>
using namespace std;


class SystemComponent{
private:
    string name;
public:
    SystemComponent(const string& componentName);
    virtual ~SystemComponent() = default;
    virtual void showStatus() = 0;
    const string& getName() const;
};

class CPU: public SystemComponent{
private:
    //int CoreCount;
    string modelName;
    double usage;
    string getModelName();
    double getCPUInfo();
public:
    CPU(const string& Cpu);

    string getModel() const;
    double getUsage() const;
    void showStatus();
};

class Memory: public SystemComponent{
private:
    string name;
    double total;
    double usage;
    double free;

    string getMemInfo();
    double getUsageInfo();
public:
    Memory(const string& Mem);
    string getMemory() const;
    double getTotal() const;
    double getUsage() const;
    double getFree() const;
    void showStatus();
};

class Disk: public SystemComponent{
private:
    string DiskName;
    double total;
    void getUsageInfo();
public:
    Disk(const string& Disk);
    string getDisk() const;
    double getTotal();
    void showStatus();
};

class Net:public SystemComponent{
private:
    string Hostname;
    string ip;
    string interface;
    void getNetInfo();
public:
    Net(const string& Net);
    string getHost() const;
    string getIp() const;
    string getInterface() const;
    void showStatus();
};

#endif // SYSTEMCOMPONENT_H
