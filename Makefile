TARGET ?= infrakit-instance-c
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name "*.c")
OBJS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJS:.o=.d)

HEADERS= -I./headers/
LIBS= -L./lib/

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) $(HEADERS) -MMD -MP

$(TARGET): $(OBJS)
	$(CC)  $(LDFLAGS) $(OBJS) $(LIBS) -ljansson -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)

-include $(DEPS)

