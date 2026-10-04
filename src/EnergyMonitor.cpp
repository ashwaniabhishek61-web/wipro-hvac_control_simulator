#include "EnergyMonitor.h"
#include <iostream>
#include <iomanip>

EnergyMonitor::EnergyMonitor(double tariffPerKWh)
    : totalCoolingKWh(0.0),
      totalHeatingKWh(0.0),
      totalStandbyKWh(0.0),
      tariffRate(tariffPerKWh) {}

void EnergyMonitor::recordCycle(Room& room, double hours) {
    double energyUsed = 0.0;
    HVACMode state = room.getHVACState();

    switch (state) {
        case HVACMode::COOLING:
            energyUsed = Config::AC_POWER_KW * hours;
            totalCoolingKWh += energyUsed;
            break;
        case HVACMode::HEATING:
            energyUsed = Config::HEATER_POWER_KW * hours;
            totalHeatingKWh += energyUsed;
            break;
        case HVACMode::ECO_STANDBY:
            energyUsed = Config::ECO_STANDBY_KW * hours;
            totalStandbyKWh += energyUsed;
            break;
        case HVACMode::OFF:
        default:
            energyUsed = 0.0;
            break;
    }

    room.addEnergy(energyUsed);
}

double EnergyMonitor::getTotalCoolingKWh() const {
    return totalCoolingKWh;
}

double EnergyMonitor::getTotalHeatingKWh() const {
    return totalHeatingKWh;
}

double EnergyMonitor::getTotalStandbyKWh() const {
    return totalStandbyKWh;
}

double EnergyMonitor::getTotalSystemKWh() const {
    return totalCoolingKWh + totalHeatingKWh + totalStandbyKWh;
}

double EnergyMonitor::getEstimatedCost() const {
    return getTotalSystemKWh() * tariffRate;
}

void EnergyMonitor::setTariffRate(double rate) {
    if (rate >= 0.0) {
        tariffRate = rate;
    }
}

void EnergyMonitor::reset() {
    totalCoolingKWh = 0.0;
    totalHeatingKWh = 0.0;
    totalStandbyKWh = 0.0;
}

void EnergyMonitor::printReport(const std::vector<Room>& rooms) const {
    std::cout << "\n==================== ENERGY CONSUMPTION REPORT ====================\n";
    std::cout << "| " << std::left << std::setw(4) << "ID"
              << "| " << std::setw(20) << "Room Name"
              << "| " << std::right << std::setw(16) << "Energy (kWh)"
              << "| " << std::setw(16) << "Estimated Cost" << " |\n";
    std::cout << "--------------------------------------------------------------------\n";

    for (const auto& room : rooms) {
        double roomCost = room.getEnergyConsumed() * tariffRate;
        std::cout << "| " << std::left << std::setw(4) << room.getId()
                  << "| " << std::setw(20) << room.getName()
                  << "| " << std::right << std::setw(12) << std::fixed << std::setprecision(3) << room.getEnergyConsumed() << " kWh "
                  << "| " << std::setw(12) << std::fixed << std::setprecision(2) << roomCost << "    |\n";
    }

    std::cout << "--------------------------------------------------------------------\n";
    std::cout << "  Breakdown by Subsystem:\n";
    std::cout << "  - AC Cooling Units : " << std::fixed << std::setprecision(3) << totalCoolingKWh << " kWh (" << (totalCoolingKWh * tariffRate) << ")\n";
    std::cout << "  - Heating Units    : " << std::fixed << std::setprecision(3) << totalHeatingKWh << " kWh (" << (totalHeatingKWh * tariffRate) << ")\n";
    std::cout << "  - Standby / Eco    : " << std::fixed << std::setprecision(3) << totalStandbyKWh << " kWh (" << (totalStandbyKWh * tariffRate) << ")\n";
    std::cout << "  Total Consumption  : " << std::fixed << std::setprecision(3) << getTotalSystemKWh() << " kWh\n";
    std::cout << "  Tariff Rate        : " << std::fixed << std::setprecision(2) << tariffRate << " per kWh\n";
    std::cout << "  Total Net Cost     : " << std::fixed << std::setprecision(2) << getEstimatedCost() << "\n";
    std::cout << "====================================================================\n";
}
