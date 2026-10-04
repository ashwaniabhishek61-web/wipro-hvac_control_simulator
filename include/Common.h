#ifndef COMMON_H
#define COMMON_H

#include <string>

// Enum representing the operating states of the HVAC system
enum class HVACMode {
    OFF,            // HVAC is turned off (comfortable temperature)
    COOLING,        // Air conditioning active
    HEATING,        // Heating unit active
    ECO_STANDBY     // Room is unoccupied; energy-saving idle mode
};

// Helper function to convert HVACMode enum to a human-readable string
inline std::string hvacModeToString(HVACMode mode) {
    switch (mode) {
        case HVACMode::OFF:         return "OFF (Comfortable)";
        case HVACMode::COOLING:     return "COOLING (AC On)";
        case HVACMode::HEATING:     return "HEATING (Heater On)";
        case HVACMode::ECO_STANDBY: return "ECO STANDBY (Unoccupied)";
        default:                    return "UNKNOWN";
    }
}

// Configurable physical thresholds and power consumption parameters
namespace Config {
    constexpr double DEFAULT_COOLING_THRESHOLD = 25.0; // °C (Above this, AC turns on)
    constexpr double DEFAULT_HEATING_THRESHOLD = 18.0; // °C (Below this, Heater turns on)
    constexpr double DEFAULT_TARGET_COMFORT    = 22.0; // °C (Ideal indoor temperature)
    
    // Power ratings in Kilowatts (kW)
    constexpr double AC_POWER_KW       = 1.50; // 1.5 kW for cooling
    constexpr double HEATER_POWER_KW   = 2.00; // 2.0 kW for heating
    constexpr double ECO_STANDBY_KW    = 0.02; // 20 W for sensor/standby circuitry

    // Simulation cycle step
    constexpr double HOURS_PER_CYCLE   = 0.25; // Each simulation step represents 15 minutes (0.25 hours)
    constexpr double TEMP_ADJUST_RATE  = 0.60; // °C adjusted towards target per cycle by HVAC
}

#endif // COMMON_H
