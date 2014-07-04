#########################
# LEAF                  #
#########################

# Parameters
# ----------------------------------------

DEBUG    ?= 1
VERBOSE  ?= 0
LDFLAGS  = -lxcb
VERSION  = $(shell cat VERSION)
V        = @
CFLAGS   = -Wall -Werror -D_POSIX_C_SOURCE -std=c99 -I.
OUT_DIR  = bin
SRC_DIR  = src
EXE_NAME = leaf
SOURCES  = leaf.c event.c window.c winmap.c

ifeq ($(DEBUG), 1)
	CFLAGS  += -g -ggdb -DVERSION="$(VERSION)-debug"
	BIN_DIR = $(OUT_DIR)/debug
else
	CFLAGS  += -Werror -Os -DNDEBUG -DVERSION="$(VERSION)"
	BIN_DIR = $(OUT_DIR)/release
endif

OBJECTS  = $(SOURCES:%.c=$(BIN_DIR)/%.o)
EXE      = $(BIN_DIR)/$(EXE_NAME)

ifeq ($(VERBOSE), 1)
	V       =
endif

# Functions
# ----------------------------------------

ifeq ($(DEBUG), 1)
	display_info = @echo -e "[\033[0;32m*\033[0;0m] $(1)"
else
	display_info = @echo -e "[\033[0;34m*\033[0;0m] $(1)"
endif

# Rules
# ----------------------------------------

all: $(EXE_NAME)

$(EXE_NAME): _start $(EXE)

_start: $(BIN_DIR)
ifneq ($(DEBUG), 1)
	$(call display_info,"--- Release mode: v$(VERSION) ---")
endif

$(BIN_DIR):
	$(V)mkdir -p $(BIN_DIR)

$(EXE): $(OBJECTS)
	$(call display_info,"LD $^ -\> $@")
	$(V)$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(SRC_DIR)/%.h
	$(call display_info,"CC $< -\> $@")
	$(V)$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(call display_info,"Cleaning all")
	$(V)$(RM) -rf bin
