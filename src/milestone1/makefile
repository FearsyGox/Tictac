CXX := g++
CXXFLAGS := -std=c++11 -Wall
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

M1_SRC := $(SRC_DIR)/tic_tac.cpp
M1_OBJ := $(M1_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
M1_TARGET := $(BIN_DIR)/milestone1_app

.PHONY: all clean milestone1 run

all: milestone1

milestone1: $(M1_TARGET)

# Compile rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link rules
$(M1_TARGET): $(M1_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: milestone1
	./$(M1_TARGET)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
