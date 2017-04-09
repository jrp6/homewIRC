#include <search.h>
#include <string.h>

#include "doJoin.h"
#include "connection.h"
#include "message.h"

static char * getChannel(char *arg)
{
  return *arg == ':' ? arg + 1 : arg;
}

static int strcmp_void(const void *a, const void *b)
{
  const char *ac = a;
  const char *bc = b;
  return strcmp(ac, bc);
}

void doJoin(const struct message msg, struct connection *conn) {
  if (msg.type == MSG_JOIN) {
    char *channel = getChannel(msg.argv[0]);
    tsearch(channel, (void **)&(conn->channels_root), strcmp_void);
  }
}

void doPart(const struct message msg, struct connection *conn) {
  if (msg.type == MSG_PART) {
    char *channel = getChannel(msg.argv[0]);
    tdelete(channel, (void **)&(conn->channels_root), strcmp_void);
  }
}
