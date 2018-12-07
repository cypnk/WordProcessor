
# Adapted from the excellent article: 
# https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

# Executable out
TARGET		?= huxley

# Sources directory
SRC_DIR		?= ./src

# Build objects directory (will be created if it doesn't exist)
BUILD_DIR	?= ./build

# Binaries location
BIN_DIR		?= ./bin

# Find sources
SRCS		:= $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Create object path
OBJS		:= $(SRCS:%=$(BUILD_DIR)/%.o)

# Dependencies
DEPS		:= $(OBJS:.o=.d)

# Include settings
INC_DIR		:= $(shell find $(SRC_DIR) -type d)
INC_FLAGS	:= $(addprefix -I,$(INC_DIR))

# C++ Flags
CXXFLAGS	?= $(INC_FLAGS) -MMD -MP -Wall -Wextra \
-Werror -pedantic -std=c++11 -O2


$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Assembly
$(BUILD_DIR)/%.s.o: $.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@
	
# C source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY:	clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
MKDIR_P		?= mkdir -p
