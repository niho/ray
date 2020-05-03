CC= gcc -std=gnu99
CFLAGS= -Wall -Wextra
DBGCFLAGS= -DDEBUG -g
RELCFLAGS= -O2

DEBUG ?= 1
ifeq ($(DEBUG), 1)
    CFLAGS += $(DBGCFLAGS)
else
    CFLAGS += $(RELCFLAGS)
endif

LDFLAGS= $(SYSLDFLAGS)
SYSLDFLAGS=
LIBS= -lm $(SYSLIBS)
SYSLIBS=

BIN= ray
BIN_O= main.o vec3.o ray.o hittable.o scene.o sphere.o camera.o color.o \
	   rand.o material.o

all: $(BIN)

$(BIN): $(BIN_O)
	$(CC) -o $@ $(LDFLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
