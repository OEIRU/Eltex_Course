CC = gcc
CFLAGS = -Wall -fPIC -Iinclude
LDFLAGS = -ldl
SRC_DIR = src
LIB_DIR = libs

OPERATIONS = add sub mul div mod pow

all: main $(OPERATIONS:%=$(LIB_DIR)/lib%.so)

main: main.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

$(LIB_DIR)/lib%.so: $(SRC_DIR)/%.c
	@mkdir -p $(LIB_DIR)
	$(CC) $(CFLAGS) -shared -o $@ $<

clean:
	rm -f main
	rm -rf $(LIB_DIR)

.PHONY: all clean
