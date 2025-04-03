# Compiler
CXX := g++
AR := ar
#CXXFLAGS := -Wall -Wextra -O2
CXXFLAGS := -w  -O2
LDFLAGS := -Lstdutil -Lcompiler -Llinker
LDLIBS := -lstdutil -lcompiler -llinker

# Directories
BLITZ_DIR := blitz
STDUTIL_DIR := stdutil
BUILD_DIR := build
RELEASE_DIR := _release/bin

COMPILER_DIR := compiler
LINKER_DIR := linker



# Find all source files
COMPILER_SRC := $(wildcard $(COMPILER_DIR)/*.cpp)
COMPILER_OBJ := $(patsubst $(COMPILER_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMPILER_SRC))

STDUTIL_SRC := $(wildcard $(STDUTIL_DIR)/*.cpp)
STDUTIL_OBJ := $(patsubst $(STDUTIL_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(STDUTIL_SRC))

BLITZ_SRC := $(wildcard $(BLITZ_DIR)/*.cpp)
BLITZ_OBJ := $(patsubst $(BLITZ_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(BLITZ_SRC))

LINKER_SRC := $(wildcard $(LINKER_DIR)/*.cpp)
LINKER_OBJ := $(patsubst $(LINKER_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(LINKER_SRC))




# Output files
STDUTIL_LIB := $(STDUTIL_DIR)/libstdutil.a
BLITZ_BIN := $(RELEASE_DIR)/blitzcc  # Changed to blitzcc

COMPILER_LIB := $(COMPILER_DIR)/libcompiler.a
LINKER_LIB := $(LINKER_DIR)/liblinker.a


# Targets
all: $(BLITZ_BIN)

# Compile stdutil as a static library
$(STDUTIL_LIB): $(STDUTIL_OBJ)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(STDUTIL_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@




# Compile compiler as a static library
$(COMPILER_LIB): $(COMPILER_OBJ)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(COMPILER_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile linker as a static library
$(LINKER_LIB): $(LINKER_OBJ)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: $(LINKER_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile blitz and link against stdutil
$(BLITZ_BIN): $(BLITZ_OBJ) $(STDUTIL_LIB) $(COMPILER_LIB) $(LINKER_LIB)
	mkdir -p $(RELEASE_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)



#$(BLITZ_BIN): $(BLITZ_OBJ) $(COMPILER_LIB)
#	mkdir -p $(RELEASE_DIR)
#	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)



$(BUILD_DIR)/%.o: $(BLITZ_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(BUILD_DIR) $(STDUTIL_LIB) $(COMPILER_LIB) $(LINKER_LIB) $(RELEASE_DIR)

.PHONY: all clean
