CC = clang
LD = clang++

CFLAGS = -Ideps

BUILD = build
BIN = poodle
SRCS = $(wildcard src/*.c)
DEPS = $(wildcard deps/*/*.c)
OBJS = $(DEPS:.c=.o) $(SRCS:.c=.o)

all: $(BUILD)/$(BIN)

$(BUILD)/$(BIN): $(OBJS)
		mkdir -p $(BUILD)
		$(LD) $^ $(LDFLAGS) -o $@

%.o: %.c
		$(CC) $< -c -o $@ $(CFLAGS)

clean:
		rm -rf $(BUILD)
		rm -f $(OBJS)
