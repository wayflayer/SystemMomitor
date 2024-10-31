CC=g++
CFLAGS=-c -Wall

all: SystemMonitor

SystemMonitor: LinuxSystem.o main.o 
	$(CC) LinuxSystem.o main.o -o SystemMonitor

LinuxSystem.o: LinuxSystem.cpp
	$(CC) $(CFLAGS) LinuxSystem.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm *.o SystemMonitor