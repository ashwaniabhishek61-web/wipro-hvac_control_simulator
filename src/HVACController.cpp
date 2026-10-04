#include "HVACController.h"
#include "Logger.h"
#include <sstream>
#include <iomanip>

HVACController::HVACController(double coolingThreshold, double heatingThreshold, double targetComfortTemp)
    : coolingThreshold(coolingThreshold),
      heatingThreshold(heatingThreshold),
      targetComfortTemp(targetComfortTemp) {}

HVACMode HVACController::determineMode(const Room& room) const {
    // Rule 1: If room is empty, enter energy-saving mode
    if (!room.isOccupied()) {
        return HVACMode::ECO_STANDBY;
    }

    // Rule 2: Room is occupied; check temperature against comfort bounds
    double temp = room.getTemperature();
    if (temp > coolingThreshold) {
        return HVACMode::COOLING;
    } else if (temp < heatingThreshold) {
        return HVACMode::HEATING;
    } else {
        // Temperature is within [heatingThreshold, coolingThreshold] -> Comfortable
        return HVACMode::OFF;
    }
}

void HVACController::controlRoom(Room& room) {
    HVACMode oldMode = room.getHVACState();
    HVACMode newMode = determineMode(room);

    // If state changed, update room state and log event
    if (oldMode != newMode) {
        room.setHVACState(newMode);
        
        std::ostringstream oss;
        oss << "Room '" << room.getName() << "' (ID: " << room.getId() << ") transitioned from ["
            << hvacModeToString(oldMode) << "] to [" << hvacModeToString(newMode) 
            << "] | Temp: " << std::fixed << std::setprecision(1) << room.getTemperature() 
            << "°C, Occupants: " << room.getOccupancy();
        
        Logger::getInstance().info(oss.str());
    }

    // Apply simulated thermodynamic effect of HVAC operating on the room
    double curTemp = room.getTemperature();
    if (newMode == HVACMode::COOLING) {
        if (curTemp > targetComfortTemp) {
            double newTemp = curTemp - Config::TEMP_ADJUST_RATE;
            if (newTemp < targetComfortTemp) newTemp = targetComfortTemp;
            room.setTemperature(newTemp);
        }
    } else if (newMode == HVACMode::HEATING) {
        if (curTemp < targetComfortTemp) {
            double newTemp = curTemp + Config::TEMP_ADJUST_RATE;
            if (newTemp > targetComfortTemp) newTemp = targetComfortTemp;
            room.setTemperature(newTemp);
        }
    }
}

double HVACController::getCoolingThreshold() const { return coolingThreshold; }
double HVACController::getHeatingThreshold() const { return heatingThreshold; }
double HVACController::getTargetComfortTemp() const { return targetComfortTemp; }

void HVACController::setCoolingThreshold(double val) { coolingThreshold = val; }
void HVACController::setHeatingThreshold(double val) { heatingThreshold = val; }
void HVACController::setTargetComfortTemp(double val) { targetComfortTemp = val; }
