#include <stdlib.h>
#include <string.h>

#include "hostmask_util.h"

char * getNick(const char *hostmask)
{
  char *nick = calloc(strcspn(hostmask, "!") + 1, sizeof(char));
  memccpy(nick, hostmask, '!', strlen(hostmask));
  return nick;
}

char * getHostmask(const char *nick, const char *user, const char *host)
{
  char *hostmask = calloc(strlen(nick) + 2 + strlen(user) + 1 + strlen(host) + 1, sizeof(char));
  strcat(hostmask, nick);
  strcat(hostmask, "!~");
  strcat(hostmask, user);
  strcat(hostmask, "@");
  strcat(hostmask, host);
  return hostmask;
}
