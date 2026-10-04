#ifndef ROOM_H
#define ROOM_H

#include "Common.h"
#include <string>

class Room {
public:
    Room(int id, const std::string& name, double initialTemp = 24.0, int initialOccupancy = 0);

    // Getters
    int getId() const;
    std::string getName() const;
    int getOccupancy() const;
    double getTemperature() const;
    HVACMode getHVACState() const;
    double getEnergyConsumed() const;

    // Setters & Mutators with input validation
    bool addOccupant(int count = 1);
    bool removeOccupant(int count = 1);
    bool setTemperature(double newTemp);
    void setHVACState(HVACMode newState);
    void addEnergy(double kWh);
    void resetEnergy();

    // Checkers
    bool isOccupied() const;

    // Display formatted room status row
    void printStatus() const;

private:
    int id;
    std::string name;
    int occupancy;
    double temperature;
    HVACMode hvacState;
    double energyConsumedKWh;

    static constexpr int MAX_OCCUPANCY = 25;
    static constexpr double MIN_VALID_TEMP = -10.0; // °C
    static constexpr double MAX_VALID_TEMP = 60.0;  // °C
};

#endif // ROOM_H
