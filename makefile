TARGET := index.html

SRC_DIR := src
INC_DIR := include
LIB_DIR := lib
OBJ_DIR := obj
ASSETS_DIR := assets
BIN_DIR := bin

ASSETS_BIN_DIR := $(BIN_DIR)/assets


CXX := em++
CXXFLAGS := -O2 -std=c++17 -Wall -I$(INC_DIR) -I$(LIB_DIR)
EMFLAGS := -s USE_WEBGL2=1 -s FULL_ES3=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy

SHELL_FILE := index.html
SRC_FILES   := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(LIB_DIR)/*.cpp)
OBJ_FILES   := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC_FILES)))

PRELOAD_FLAG := --preload-file $(ASSETS_DIR)@/assets

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	@echo "Linking"
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) $(CXXFLAGS) $(EMFLAGS) $(PRELOAD_FLAG) --shell-file $(SHELL_FILE) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

assets:
	@echo "Rebuilding assets"
	@mkdir -p $(BIN_DIR)
	@rm -f $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(TARGET:.html=.data) $(BIN_DIR)/$(TARGET:.html=.wasm)
	$(CXX) $(OBJ_FILES) $(CXXFLAGS) $(EMFLAGS) $(PRELOAD_FLAG) --shell-file $(SHELL_FILE) -o $(BIN_DIR)/$(TARGET)

clean:
	@echo "Cleaning"
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean assets
