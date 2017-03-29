#include <stdlib.h>
#include <string.h>
#include "message.h"

void freeMessage(struct message msg) {
  for (unsigned int i = 0; i < msg.argc; ++i) {
    free((msg.argv)[i]);
  }
  if (msg.argv) {
    free(msg.argv);
  }
}

static char * enumtostr(const enum message_type t) {
  switch (t) {
  case MSG_NICK:
    return "NICK";
    break;
  case MSG_USER:
    return "USER";
    break;
  case MSG_PRIVMSG:
    return "PRIVMSG";
    break;
  case MSG_JOIN:
    return "JOIN";
    break;
  case MSG_PART:
    return "PART";
    break;
  case MSG_QUIT:
    return "QUIT";
    break;
  case MSG_INVALID:
  default:
    break;
  }

  return "INVALID_MSG";
}

static enum message_type strtoenum(const char *str) {
  if (strcmp(str, "NICK") == 0) {
    return MSG_NICK;
  }
  if (strcmp(str, "USER") == 0) {
    return MSG_USER;
  }
  if (strcmp(str, "PRIVMSG") == 0) {
    return MSG_PRIVMSG;
  }
  if (strcmp(str, "JOIN") == 0) {
    return MSG_JOIN;
  }
  if (strcmp(str, "PART") == 0) {
    return MSG_PART;
  }
  if (strcmp(str, "QUIT") == 0) {
    return MSG_QUIT;
  }
  return MSG_INVALID;
}

struct message parseMessage(const char *msgstr_)
{
  char *msgstr = malloc((strlen(msgstr_) + 1) * sizeof(char));
  strcpy(msgstr, msgstr_);

  struct message msg;
  msg.type = MSG_INVALID;
  msg.argc = 0;
  msg.argv = NULL;

  // Remove terminating \r\n or \n
  // Kinda ugly and repetitive, TODO: make less ugly
  char *terminator = strstr(msgstr, "\r\n");
  if (terminator) {
    *terminator = '\0';
  } else if ((terminator = strchr(msgstr, '\n'))) {
    *terminator = '\0';
  }

  const char *delim = " ";
  char **saveptr = malloc(sizeof(char *));

  // Get the message type
  char *typeStr = strtok_r(msgstr, delim, saveptr);
  msg.type = strtoenum(typeStr);

  // Parse the arguments
  unsigned int argv_size = 0;
  char *arg = strtok_r(NULL, delim, saveptr);
  while (arg) {
    if (msg.argc == argv_size) {
      argv_size = (argv_size == 0) ? 1 : 2 * argv_size;
      msg.argv = realloc(msg.argv, argv_size * sizeof(char *));
    }

    msg.argv[msg.argc] = malloc((strlen(arg) + 1) * sizeof(char));
    strcpy(msg.argv[msg.argc], arg);

    ++(msg.argc);
    arg = strtok_r(NULL, delim, saveptr);
  }

  free(msgstr);
  free(saveptr);
  return msg;
}

char * stringifyMessage(const struct message msg)
{
  unsigned int strsize = 16;
  char *str = calloc(strsize, sizeof(char));

  // Get the message type
  strncat(str, enumtostr(msg.type), strsize - 1);
  strcat(str, " ");

  // Add the arguments
  // This could be optimized by not repeatedly calling strcat
  for (unsigned int i = 0; i < msg.argc; ++i) {
    char *arg = msg.argv[i];
    if (strlen(str) + strlen(arg) + 1 >= strsize) {
      strsize *= 2;
      str = realloc(str, strsize * sizeof(char));
    }
    strcat(str, arg);
    strcat(str, " ");
  }

  // Remove trailing space
  str[strlen(str) - 1] = '\0';

  strcat(str, "\r\n");
  return str;
}
