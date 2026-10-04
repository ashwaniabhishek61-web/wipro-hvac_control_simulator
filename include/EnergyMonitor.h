#ifndef ENERGY_MONITOR_H
#define ENERGY_MONITOR_H

#include "Room.h"
#include "Common.h"
#include <vector>

class EnergyMonitor {
public:
    EnergyMonitor(double tariffPerKWh = 8.00); // Standard domestic tariff per kWh (e.g. INR 8.00)

    // Records energy consumed during a single simulation cycle step
    void recordCycle(Room& room, double hours = Config::HOURS_PER_CYCLE);

    // Getters for aggregated statistics
    double getTotalCoolingKWh() const;
    double getTotalHeatingKWh() const;
    double getTotalStandbyKWh() const;
    double getTotalSystemKWh() const;
    double getEstimatedCost() const;

    // Setters
    void setTariffRate(double rate);

    // Formatted report output
    void printReport(const std::vector<Room>& rooms) const;

    // Reset monitor counters
    void reset();

private:
    double totalCoolingKWh;
    double totalHeatingKWh;
    double totalStandbyKWh;
    double tariffRate; // Currency units per kWh
};

#endif // ENERGY_MONITOR_H
