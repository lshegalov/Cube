TARGET_EXEC := CubeSplit

BUILD_DIR := ./build
SRC_DIRS := .

CPPFLAGS= -std=c++17 -Werror -Ofast -Wall -DNDEBUG -MMD -MP
LDFLAGS=-L/usr/lib
LDLIBS=-lstdc++ -lm

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

   
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(LDFLAGS)  $(OBJS) -o $@  $(LDLIBS)

all: $(TARGET_EXEC)


.PHONY: clean
clean:
	rm -r $(BUILD_DIR)



# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)