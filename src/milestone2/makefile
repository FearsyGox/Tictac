CXX := g++
CXXFLAGS := -std=c++11 -Wall
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

M2_SRC := $(SRC_DIR)/main.cpp $(SRC_DIR)/header.h $(SRC_DIR)/client.h $(SRC_DIR)/server.h $(SRC_DIR)/game.h
M2_OBJ := $(M2_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
M2_TARGET := $(BIN_DIR)/milestone2_app

.PHONY: all clean milestone2 run

all: milestone2

milestone2: $(M2_TARGET)

# Compile rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link rules
$(M2_TARGET): $(M2_OBJ)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: milestone2
	./$(M2_TARGET)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
