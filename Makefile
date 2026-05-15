#Project config
CXX      := g++

CXXFLAGS := -O3 -Wall -Ilib/imgui -Ilib/imgui-sfml

LDFLAGS  := -lsfml-graphics -lsfml-window -lsfml-system -lGL -lsfml-audio

#Dir structure
SRC_DIR  := src
OBJ_DIR  := obj
BIN_DIR  := bin
LIB_DIR  := lib

# Paths para as libs
IMGUI_DIR      := $(LIB_DIR)/imgui
IMGUI_SFML_DIR := $(LIB_DIR)/imgui-sfml

# Files
# 1. Seus arquivos do jogo (src/*.cpp)
PROJECT_SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
# 2. Arquivos do ImGui (lib/imgui/*.cpp)
IMGUI_SOURCES   := $(wildcard $(IMGUI_DIR)/*.cpp)
# 3. Arquivos do ImGui-SFML (lib/imgui-sfml/*.cpp)
SFML_IMGUI_SOURCES := $(wildcard $(IMGUI_SFML_DIR)/*.cpp)

# Objects Generation
# Mapeia src/%.cpp -> obj/%.o
PROJECT_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(PROJECT_SOURCES))
# Mapeia lib/imgui/%.cpp -> obj/%.o
IMGUI_OBJECTS   := $(patsubst $(IMGUI_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(IMGUI_SOURCES))
# Mapeia lib/imgui-sfml/%.cpp -> obj/%.o
SFML_IMGUI_OBJECTS := $(patsubst $(IMGUI_SFML_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SFML_IMGUI_SOURCES))

# Lista final de todos os objetos para o Linker
ALL_OBJECTS := $(PROJECT_OBJECTS) $(IMGUI_OBJECTS) $(SFML_IMGUI_OBJECTS)

# Name of the final executable
TARGET   := $(BIN_DIR)/game.bin

# Rules of compilation && execution

# Standard rule
all: $(TARGET)

# 1. Linking: Unite all .o files into the executable
$(TARGET): $(ALL_OBJECTS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking..."
	$(CXX) $(ALL_OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build complete: $@"

# 2. Compilation Rules

# Regra para seus arquivos (src/)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling Project: $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para ImGui (lib/imgui/)
$(OBJ_DIR)/%.o: $(IMGUI_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling ImGui: $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para ImGui-SFML (lib/imgui-sfml/)
$(OBJ_DIR)/%.o: $(IMGUI_SFML_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling ImGui-SFML: $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Running Rules
run: all
	@echo "Running Game..."
	@./$(TARGET)

rund: all
	@echo "Running Game with Debug..."
	@./$(TARGET) -d

# Cleaning Rules
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.ini
	@echo "Cleaned!"

.PHONY: all run clean