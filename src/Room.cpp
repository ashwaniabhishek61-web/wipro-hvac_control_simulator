#include "Room.h"
#include <iostream>
#include <iomanip>

Room::Room(int id, const std::string& name, double initialTemp, int initialOccupancy)
    : id(id),
      name(name),
      occupancy(initialOccupancy >= 0 ? initialOccupancy : 0),
      temperature(initialTemp),
      hvacState(HVACMode::OFF),
      energyConsumedKWh(0.0) {}

int Room::getId() const {
    return id;
}

std::string Room::getName() const {
    return name;
}

int Room::getOccupancy() const {
    return occupancy;
}

double Room::getTemperature() const {
    return temperature;
}

HVACMode Room::getHVACState() const {
    return hvacState;
}

double Room::getEnergyConsumed() const {
    return energyConsumedKWh;
}

bool Room::isOccupied() const {
    return occupancy > 0;
}

bool Room::addOccupant(int count) {
    if (count <= 0) {
        std::cerr << "[Validation Error] Occupant count to add must be positive.\n";
        return false;
    }
    if (occupancy + count > MAX_OCCUPANCY) {
        std::cerr << "[Validation Error] Exceeds maximum room capacity of " 
                  << MAX_OCCUPANCY << " people.\n";
        return false;
    }
    occupancy += count;
    return true;
}

bool Room::removeOccupant(int count) {
    if (count <= 0) {
        std::cerr << "[Validation Error] Occupant count to remove must be positive.\n";
        return false;
    }
    if (occupancy - count < 0) {
        std::cerr << "[Validation Error] Cannot remove more occupants than present (Current: " 
                  << occupancy << ").\n";
        return false;
    }
    occupancy -= count;
    return true;
}

bool Room::setTemperature(double newTemp) {
    if (newTemp < MIN_VALID_TEMP || newTemp > MAX_VALID_TEMP) {
        std::cerr << "[Validation Error] Temperature " << newTemp 
                  << "°C is outside realistic bounds [" 
                  << MIN_VALID_TEMP << "°C to " << MAX_VALID_TEMP << "°C].\n";
        return false;
    }
    temperature = newTemp;
    return true;
}

void Room::setHVACState(HVACMode newState) {
    hvacState = newState;
}

void Room::addEnergy(double kWh) {
    if (kWh > 0.0) {
        energyConsumedKWh += kWh;
    }
}

void Room::resetEnergy() {
    energyConsumedKWh = 0.0;
}

void Room::printStatus() const {
    std::cout << "| " << std::setw(3) << id << " "
              << "| " << std::left << std::setw(16) << name << std::right
              << "| " << std::setw(9) << occupancy << " "
              << "| " << std::setw(10) << std::fixed << std::setprecision(1) << temperature << "°C "
              << "| " << std::left << std::setw(24) << hvacModeToString(hvacState) << std::right
              << "| " << std::setw(11) << std::fixed << std::setprecision(3) << energyConsumedKWh << " kWh |\n";
}
