#ifndef _MESSAGE_H
#define _MESSAGE_H

enum message_type {
  MSG_INVALID,
  MSG_NICK,
  MSG_USER,
  MSG_PRIVMSG,
  MSG_JOIN,
  MSG_PART,
  MSG_QUIT,
};

struct message {
  enum message_type type;
  unsigned int argc;
  char **argv;
};

void freeMessage(struct message msg);

// Parse a message terminated by \0, \r\n or \n
struct message parseMessage(const char* msgstr);
// Turn a message struct into a \r\n terminated string
char * stringifyMessage(const struct message msg);

#endif
