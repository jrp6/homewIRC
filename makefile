CC=gcc
CFLAGS=-c -iquote common -O -g -Wextra -Wall -Werror -Wfloat-equal -Wshadow -Wstrict-prototypes -Wswitch-enum
LDFLAGS=-levent

SOURCES_COMMON = common/close_on_error_eof_cb.c common/logMessage.c common/message.c common/replyPing.c common/hostmask_util.c
SOURCES_CLIENT = client/main.c client/clientRead_cb.c client/nick_on_connect_cb.c client/command_cb.c
SOURCES_SERVER = server/main.c server/accept_cb.c server/connection.c server/serverRead_cb.c

OBJECTS_COMMON = $(SOURCES_COMMON:.c=.o)
OBJECTS_CLIENT = $(SOURCES_CLIENT:.c=.o)
OBJECTS_SERVER = $(SOURCES_SERVER:.c=.o)

EXECUTABLE_CLIENT = homewirc
EXECUTABLE_SERVER = homewircd

all: $(EXECUTABLE_CLIENT) $(EXECUTABLE_SERVER)

$(EXECUTABLE_CLIENT): $(OBJECTS_COMMON) $(OBJECTS_CLIENT)
	$(CC) $(OBJECTS_COMMON) $(OBJECTS_CLIENT) $(LDFLAGS) -o $@

$(EXECUTABLE_SERVER): $(OBJECTS_COMMON) $(OBJECTS_SERVER)
	$(CC) $(OBJECTS_COMMON) $(OBJECTS_SERVER) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJECTS_COMMON) $(OBJECTS_CLIENT) $(OBJECTS_SERVER) $(EXECUTABLE_CLIENT) $(EXECUTABLE_SERVER)
