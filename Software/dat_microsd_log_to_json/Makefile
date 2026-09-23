CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99 -Iinclude/
LIBS=-lm

EXEC_FILE = dat_log_to_json
CFILES = dat_log_to_json.c
CFILES += src/print_json.c

OFILES_DIR = build/
OFILES = $(CFILES:%.c=build/%.o)

build: $(EXEC_FILE)

$(EXEC_FILE): $(OFILES)
	$(CC) -o $@ $(OFILES) $(LDFLAGS)

$(OFILES): build/%.o : %.c

build/%.o : %.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OFILES) $(EXEC_FILE) $(OFILES_DIR)

