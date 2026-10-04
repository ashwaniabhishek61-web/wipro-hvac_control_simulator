#ifndef HVAC_CONTROLLER_H
#define HVAC_CONTROLLER_H

#include "Room.h"
#include "Common.h"

class HVACController {
public:
    HVACController(double coolingThreshold = Config::DEFAULT_COOLING_THRESHOLD,
                   double heatingThreshold = Config::DEFAULT_HEATING_THRESHOLD,
                   double targetComfortTemp = Config::DEFAULT_TARGET_COMFORT);

    // Determines the appropriate HVAC mode according to occupancy and temperature
    HVACMode determineMode(const Room& room) const;

    // Evaluates the room, updates its HVAC state, logs transitions, and applies simulated temperature regulation
    void controlRoom(Room& room);

    // Configuration getters & setters
    double getCoolingThreshold() const;
    double getHeatingThreshold() const;
    double getTargetComfortTemp() const;

    void setCoolingThreshold(double val);
    void setHeatingThreshold(double val);
    void setTargetComfortTemp(double val);

private:
    double coolingThreshold;
    double heatingThreshold;
    double targetComfortTemp;
};

#endif // HVAC_CONTROLLER_H
