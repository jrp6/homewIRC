#include <stdlib.h>
#include <string.h>
#include "message.h"

void freeMessage(struct message msg) {
  if (msg.source) {
    free(msg.source);
  }
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
  char *msgstr = calloc((strlen(msgstr_) + 1), sizeof(char));
  strcpy(msgstr, msgstr_);

  struct message msg;
  msg.source = NULL;
  msg.type = MSG_INVALID;
  msg.argc = 0;
  msg.argv = NULL;

  // Remove terminating \r\n or \n
  char *terminator;
  if ((terminator = strstr(msgstr, "\r\n")) || (terminator = strchr(msgstr, '\n'))) {
    *terminator = '\0';
  }

  // Get the last argument. Skip the first character in case it is a : for the source
  char *last = NULL;
  const char *lastStart = strchr(msgstr + 1, ':');
  if (lastStart) {
    last = calloc(strlen(lastStart), sizeof(char));
    strcpy(last, lastStart + 1);
  }

  const char *delim = " ";
  char **saveptr = malloc(sizeof(char *));

  // Get the source and type
  char *typeStr;
  char *source = strtok_r(msgstr, delim, saveptr);
  if (*source == ':') {
    typeStr = strtok_r(NULL, delim, saveptr);

    ++source;
    msg.source = malloc((strlen(source) + 1) * sizeof(char));
    strcpy(msg.source, source);
  } else {
    typeStr = source;
  }

  msg.type = strtoenum(typeStr);

  // Parse the arguments
  unsigned int argv_size = 0;
  char *arg = strtok_r(NULL, delim, saveptr);
  while (arg) {
    if (msg.argc == argv_size) {
      argv_size = (argv_size == 0) ? 1 : 2 * argv_size;
      msg.argv = realloc(msg.argv, argv_size * sizeof(char *));
    }
    if (*arg == ':') {
      break;
    }

    msg.argv[msg.argc] = malloc((strlen(arg) + 1) * sizeof(char));
    strcpy(msg.argv[msg.argc], arg);

    ++(msg.argc);
    arg = strtok_r(NULL, delim, saveptr);
  }

  if (last) {
    msg.argv[msg.argc] = last;
    ++(msg.argc);
  }

  free(msgstr);
  free(saveptr);
  return msg;
}

char * stringifyMessage(const struct message msg)
{
  unsigned int strsize = (msg.source ? strlen(msg.source) : 0) + 16;
  char *str = calloc(strsize, sizeof(char));

  // Get the message source
  if (msg.source) {
    *str = ':';
    strncat(str, msg.source, strsize - 1);
    strcat(str, " ");
  }

  // Get the message type
  strncat(str, enumtostr(msg.type), strsize - strlen(str) - 1);
  strcat(str, " ");

  // Add the arguments
  // This could be optimized by not repeatedly calling strcat
  for (unsigned int i = 0; i < msg.argc; ++i) {
    char *arg = msg.argv[i];
    while (strlen(str) + strlen(arg) + 1 >= strsize) {
      strsize *= 2;
    }
    str = realloc(str, strsize * sizeof(char));
    strcat(str, arg);
    strcat(str, " ");
  }

  // Remove trailing space
  str[strlen(str) - 1] = '\0';

  strcat(str, "\r\n");
  return str;
}
