CC = g++
CFLAGS = -w -g -fPIC

# Output folders
OUT_DIR = _release/bin
LIB_DIR = _release/lib
BUILD_DIR = build

# Source folders
LINKER_SRC_DIR = linker
LINKER_DLL_SRC_DIR = linker_dll
TEST_SRC_DIR = blitz
STDUTIL_SRC_DIR = stdutil
#COMPILER_SRC_DIR = stdutil


# Source files
LINKER_SRCS = $(wildcard $(LINKER_SRC_DIR)/*.cpp)
LINKER_DLL_SRCS = $(wildcard $(LINKER_DLL_SRC_DIR)/*.cpp)
TEST_SRCS = $(wildcard $(TEST_SRC_DIR)/*.cpp)
STDUTIL_SRCS = $(wildcard $(STDUTIL_SRC_DIR)/*.cpp)
#COMPILER_SRCS = $(wildcard $(COMPILER_SRC_DIR)/*.cpp)

# Object files
STDUTIL_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(STDUTIL_SRCS)))
LINKER_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(LINKER_SRCS)))
LINKER_DLL_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(LINKER_DLL_SRCS)))
TEST_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(TEST_SRCS)))
#COMPILER_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(COMPILER_SRCS)))




# Targets
STDUTIL_LIB = $(LIB_DIR)/libstdutil.a
LINKER_SO = $(OUT_DIR)/linker.so
TEST_BIN = $(OUT_DIR)/test

all: $(LINKER_SO) $(TEST_BIN)

$(OUT_DIR) $(LIB_DIR) $(BUILD_DIR):
	mkdir -p $@

# Compile each .cpp into .o
$(BUILD_DIR)/%.o: $(STDUTIL_SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LINKER_SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(LINKER_DLL_SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#$(BUILD_DIR)/%.o: $(COMPILER_SRC_DIR)/%.cpp | $(BUILD_DIR)
#	$(CC) $(CFLAGS) -c $< -o $@

# Static lib from stdutil .o files
$(STDUTIL_LIB): $(STDUTIL_OBJS) | $(LIB_DIR)
	ar rcs $@ $^

# linker.so links stdutil too
$(LINKER_SO): $(LINKER_OBJS) $(LINKER_DLL_OBJS) $(STDUTIL_LIB) | $(OUT_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $(LINKER_OBJS) $(LINKER_DLL_OBJS) $(STDUTIL_LIB)

# test app links stdutil too $(COMPILER_OBJS) 
$(TEST_BIN): $(TEST_OBJS) $(STDUTIL_LIB) | $(OUT_DIR)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(STDUTIL_LIB)

clean:
	rm -rf $(OUT_DIR) $(LIB_DIR) $(BUILD_DIR)
