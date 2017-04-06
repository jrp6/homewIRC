#ifndef _MESSAGE_H
#define _MESSAGE_H

enum message_type {
  MSG_INVALID,
  MSG_NICK,
  MSG_USER,
  MSG_PING,
  MSG_PONG,
  MSG_PRIVMSG,
  MSG_JOIN,
  MSG_PART,
  MSG_QUIT,
  MSG_CMD_CHANNEL,
};

struct message {
  char *source;
  enum message_type type;
  unsigned int argc;
  char **argv;
};

void freeMessage(struct message msg);
int isLocal(enum message_type type);

// Parse a message terminated by \0, \r\n or \n
struct message parseMessage(const char* msgstr);
// Turn a message struct into a \r\n terminated string
char * stringifyMessage(const struct message msg);

#endif
