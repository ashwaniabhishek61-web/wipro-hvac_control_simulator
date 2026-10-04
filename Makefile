# ==============================================================================
# Makefile for Smart Home Occupancy & HVAC Control Simulator
# Compatible with Linux (native/WSL) and MinGW on Windows
# ==============================================================================

CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -std=c++17 -Iinclude

SRCDIR = src
INCDIR = include
LOGDIR = logs

# List of source files
SRCS = $(SRCDIR)/main.cpp \
       $(SRCDIR)/Room.cpp \
       $(SRCDIR)/HVACController.cpp \
       $(SRCDIR)/EnergyMonitor.cpp \
       $(SRCDIR)/Logger.cpp \
       $(SRCDIR)/Simulator.cpp

OBJS = $(SRCS:.cpp=.o)

TARGET = simulator

# Default build target
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "[LINK] Linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
	@echo "[SUCCESS] Build complete: ./$(TARGET)"

%.o: %.cpp
	@echo "[COMPILE] Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "[CLEAN] Removing object files and binary..."
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
