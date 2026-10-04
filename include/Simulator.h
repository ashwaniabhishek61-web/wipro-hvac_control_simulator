#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Room.h"
#include "HVACController.h"
#include "EnergyMonitor.h"
#include <vector>

class Simulator {
public:
    Simulator();

    // Setup initial default smart home rooms
    void initializeDefaultRooms();

    // Advance the simulation by N cycles
    void advanceCycle(int cycles = 1);

    // Terminal UI methods
    void displayDashboard() const;
    void showMenu() const;
    void runInteractiveLoop();

    // User action workflows
    void handleAddOccupant();
    void handleRemoveOccupant();
    void handleChangeTemperature();
    void handleConfigureThresholds();
    void handleViewEnergy();
    void handleViewLogs();

private:
    std::vector<Room> rooms;
    HVACController controller;
    EnergyMonitor energyMonitor;
    int currentCycle;

    // Helper utilities
    Room* findRoomById(int id);
    const Room* findRoomById(int id) const;

    // Input validation helpers
    static int readInt(const std::string& prompt, int minVal, int maxVal);
    static double readDouble(const std::string& prompt, double minVal, double maxVal);
};

#endif // SIMULATOR_H
