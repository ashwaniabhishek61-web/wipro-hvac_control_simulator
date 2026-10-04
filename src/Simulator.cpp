#include "Simulator.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

Simulator::Simulator() : currentCycle(0) {
    initializeDefaultRooms();
}

void Simulator::initializeDefaultRooms() {
    rooms.clear();
    // ID, Name, Initial Temp (°C), Initial Occupancy
    rooms.emplace_back(101, "Living Room",    26.5, 2);
    rooms.emplace_back(102, "Master Bedroom", 21.0, 1);
    rooms.emplace_back(103, "Kitchen",        28.0, 0);
    rooms.emplace_back(104, "Study / Office", 17.5, 1);

    Logger::getInstance().info("Initialized 4 standard smart home rooms.");
    
    // Run initial assessment so initial states match
    for (auto& room : rooms) {
        controller.controlRoom(room);
    }
}

Room* Simulator::findRoomById(int id) {
    for (auto& room : rooms) {
        if (room.getId() == id) return &room;
    }
    return nullptr;
}

const Room* Simulator::findRoomById(int id) const {
    for (const auto& room : rooms) {
        if (room.getId() == id) return &room;
    }
    return nullptr;
}

int Simulator::readInt(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "[Error] Input out of range (" << minVal << " to " << maxVal << "). Try again.\n";
        } else {
            std::cout << "[Error] Invalid input. Please enter an integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

double Simulator::readDouble(const std::string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                return value;
            }
            std::cout << "[Error] Input out of range (" << minVal << " to " << maxVal << "). Try again.\n";
        } else {
            std::cout << "[Error] Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

void Simulator::displayDashboard() const {
    std::cout << "\n================================ SMART HOME DASHBOARD ================================\n";
    std::cout << " Process ID (PID)   : " << Logger::getInstance().getProcessId() << "\n";
    std::cout << " Simulation Cycle   : " << currentCycle 
              << " (" << (currentCycle * Config::HOURS_PER_CYCLE) << " hours simulated)\n";
    std::cout << " Cooling Threshold  : > " << controller.getCoolingThreshold() << "°C (Turns AC On)\n";
    std::cout << " Heating Threshold  : < " << controller.getHeatingThreshold() << "°C (Turns Heater On)\n";
    std::cout << " Comfort Target     : " << controller.getTargetComfortTemp() << "°C\n";
    std::cout << "+-----+------------------+-----------+------------+--------------------------+-------------+\n";
    std::cout << "| ID  | Room Name        | Occupancy | Temp (°C)  | HVAC Mode Status         | Energy Used |\n";
    std::cout << "+-----+------------------+-----------+------------+--------------------------+-------------+\n";

    for (const auto& room : rooms) {
        room.printStatus();
    }
    std::cout << "+-----+------------------+-----------+------------+--------------------------+-------------+\n";
}

void Simulator::advanceCycle(int cycles) {
    if (cycles <= 0) cycles = 1;

    for (int c = 0; c < cycles; ++c) {
        currentCycle++;
        for (auto& room : rooms) {
            // 1. Evaluate & regulate temperature with HVAC
            controller.controlRoom(room);
            // 2. Track energy for this 15-minute slice
            energyMonitor.recordCycle(room, Config::HOURS_PER_CYCLE);
        }
    }

    std::ostringstream oss;
    oss << "Advanced simulation by " << cycles << " cycle(s). Current cycle: " << currentCycle;
    Logger::getInstance().info(oss.str());

    std::cout << "\n[INFO] Simulation advanced by " << cycles 
              << " cycle(s) (" << (cycles * Config::HOURS_PER_CYCLE) << " hr). HVAC decisions applied.\n";
}

void Simulator::handleAddOccupant() {
    int id = readInt("Enter Room ID to add occupants: ", 100, 999);
    Room* room = findRoomById(id);
    if (!room) {
        std::cout << "[Error] Room ID " << id << " not found.\n";
        return;
    }

    int count = readInt("Enter number of persons entering: ", 1, 20);
    if (room->addOccupant(count)) {
        std::ostringstream oss;
        oss << "Added " << count << " occupant(s) to " << room->getName() 
            << ". Total occupancy now: " << room->getOccupancy();
        Logger::getInstance().info(oss.str());
        std::cout << "[SUCCESS] " << oss.str() << "\n";
        
        // Immediate HVAC re-evaluation
        controller.controlRoom(*room);
    }
}

void Simulator::handleRemoveOccupant() {
    int id = readInt("Enter Room ID to remove occupants: ", 100, 999);
    Room* room = findRoomById(id);
    if (!room) {
        std::cout << "[Error] Room ID " << id << " not found.\n";
        return;
    }

    if (room->getOccupancy() == 0) {
        std::cout << "[Warning] Room is already empty (0 occupants).\n";
        return;
    }

    int count = readInt("Enter number of persons leaving: ", 1, room->getOccupancy());
    if (room->removeOccupant(count)) {
        std::ostringstream oss;
        oss << "Removed " << count << " occupant(s) from " << room->getName() 
            << ". Total occupancy now: " << room->getOccupancy();
        Logger::getInstance().info(oss.str());
        std::cout << "[SUCCESS] " << oss.str() << "\n";

        // Immediate HVAC re-evaluation
        controller.controlRoom(*room);
    }
}

void Simulator::handleChangeTemperature() {
    int id = readInt("Enter Room ID to change temperature: ", 100, 999);
    Room* room = findRoomById(id);
    if (!room) {
        std::cout << "[Error] Room ID " << id << " not found.\n";
        return;
    }

    double newTemp = readDouble("Enter new temperature in °C (-10.0 to 55.0): ", -10.0, 55.0);
    double oldTemp = room->getTemperature();
    if (room->setTemperature(newTemp)) {
        std::ostringstream oss;
        oss << "Temperature in " << room->getName() << " manually changed from " 
            << oldTemp << "°C to " << newTemp << "°C.";
        Logger::getInstance().info(oss.str());
        std::cout << "[SUCCESS] " << oss.str() << "\n";

        // Immediate HVAC re-evaluation
        controller.controlRoom(*room);
    }
}

void Simulator::handleConfigureThresholds() {
    std::cout << "\n--- Configure HVAC Temperature Setpoints ---\n";
    double heat = readDouble("Enter Heating Threshold (< this turns Heater ON, e.g. 18.0°C): ", 5.0, 25.0);
    double cool = readDouble("Enter Cooling Threshold (> this turns AC ON, e.g. 25.0°C): ", heat + 1.0, 40.0);
    double comfort = readDouble("Enter Target Comfort Temperature (e.g. 22.0°C): ", heat, cool);

    controller.setHeatingThreshold(heat);
    controller.setCoolingThreshold(cool);
    controller.setTargetComfortTemp(comfort);

    std::ostringstream oss;
    oss << "Thresholds updated: Heat < " << heat << "°C, Cool > " << cool 
        << "°C, Comfort Target = " << comfort << "°C";
    Logger::getInstance().info(oss.str());
    std::cout << "[SUCCESS] " << oss.str() << "\n";

    // Re-evaluate all rooms under new thresholds
    for (auto& room : rooms) {
        controller.controlRoom(room);
    }
}

void Simulator::handleViewEnergy() {
    energyMonitor.printReport(rooms);
}

void Simulator::handleViewLogs() {
    Logger::getInstance().displayRecentLogs(15);
}

void Simulator::showMenu() const {
    std::cout << "\n================= OPERATOR CONTROL MENU =================\n";
    std::cout << " 1. View Dashboard\n";
    std::cout << " 2. Add Person to Room\n";
    std::cout << " 3. Remove Person from Room\n";
    std::cout << " 4. Change Room Temperature\n";
    std::cout << " 5. Run 1 Simulation Cycle Step (15 mins)\n";
    std::cout << " 6. Run Multiple Simulation Steps\n";
    std::cout << " 7. View Energy Consumption Report\n";
    std::cout << " 8. Configure HVAC Setpoints/Thresholds\n";
    std::cout << " 9. View Activity Log History\n";
    std::cout << " 0. Exit Simulator\n";
    std::cout << "=========================================================\n";
}

void Simulator::runInteractiveLoop() {
    int choice = -1;
    displayDashboard();

    while (choice != 0) {
        showMenu();
        choice = readInt("Select an option [0-9]: ", 0, 9);

        switch (choice) {
            case 1:
                displayDashboard();
                break;
            case 2:
                handleAddOccupant();
                displayDashboard();
                break;
            case 3:
                handleRemoveOccupant();
                displayDashboard();
                break;
            case 4:
                handleChangeTemperature();
                displayDashboard();
                break;
            case 5:
                advanceCycle(1);
                displayDashboard();
                break;
            case 6: {
                int steps = readInt("Enter number of cycles to advance (1-96): ", 1, 96);
                advanceCycle(steps);
                displayDashboard();
                break;
            }
            case 7:
                handleViewEnergy();
                break;
            case 8:
                handleConfigureThresholds();
                displayDashboard();
                break;
            case 9:
                handleViewLogs();
                break;
            case 0:
                std::cout << "\nTerminating Smart Home Simulator. Goodbye!\n";
                Logger::getInstance().info("User requested session termination.");
                break;
            default:
                std::cout << "[Error] Invalid option.\n";
                break;
        }
    }
}
