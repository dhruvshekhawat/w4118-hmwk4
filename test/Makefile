CC := arm-none-linux-gnueabi-gcc
LD := arm-none-linux-gnueabi-gcc
CFLAGS := -g -Wall -Werror
LDFLAGS := -static

OBJECTS := set_sched_policy.c
EXE := set_sched_policy

all : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXE) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f $(EXE)

.PHONY: clean
