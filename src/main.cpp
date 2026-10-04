#include "Simulator.h"
#include "Logger.h"
#include <iostream>
#include <csignal>
#include <cstdlib>

// Linux Signal Handler for graceful termination (e.g., Ctrl+C / SIGINT)
void signalHandler(int signum) {
    std::cout << "\n\n[SIGNAL] Interrupt signal (" << signum 
              << ") received. Flushing buffers and terminating safely...\n";
    Logger::getInstance().warn("Termination signal caught by process.");
    exit(signum);
}

int main() {
    // Register signal handler for SIGINT (Ctrl+C) and SIGTERM
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "====================================================================\n";
    std::cout << "         SMART HOME OCCUPANCY & HVAC CONTROL SIMULATOR              \n";
    std::cout << "         Platform : Linux / POSIX C++ System Simulation             \n";
    std::cout << "====================================================================\n";

    // Initialize logger singleton
    Logger::getInstance().info("System starting up. Initializing modules...");

    try {
        Simulator simulator;
        simulator.runInteractiveLoop();
    } catch (const std::exception& ex) {
        std::cerr << "[CRITICAL EXCEPTION] " << ex.what() << "\n";
        Logger::getInstance().error(std::string("Unhandled exception: ") + ex.what());
        return 1;
    } catch (...) {
        std::cerr << "[CRITICAL EXCEPTION] Unknown exception occurred.\n";
        Logger::getInstance().error("Unknown fatal error encountered.");
        return 1;
    }

    Logger::getInstance().info("System terminated gracefully with return code 0.");
    return 0;
}
