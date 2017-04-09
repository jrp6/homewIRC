#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "defaults.h"
#ifdef SERVER_DEBUG
#include <stdio.h>
#endif

#include "doJoin.h"
#include "connection.h"
#include "message.h"
#include "strcmp_void.h"

static char * getChannel(char *arg)
{
  return *arg == ':' ? arg + 1 : arg;
}

#ifdef SERVER_DEBUG
static void printChannel_cb(const void *nodep, const VISIT which, const int depth)
{
  (void)depth;
  if (which == postorder || which == leaf) {
    const char *node = *(char **)nodep;
    puts(node);
  }
}
#endif

void doJoin(const struct message msg, struct connection *conn) {
  if (msg.type == MSG_JOIN) {
    char *channel_ = getChannel(msg.argv[0]);
    char *channel = malloc((strlen(channel_) + 1) * sizeof(char));
    strcpy(channel, channel_);
    tsearch(channel, (void **)&(conn->channels_root), strcmp_void);
#ifdef SERVER_DEBUG
    printf("Conn %u: Joined %s\n", conn->id, channel);
    twalk(conn->channels_root, printChannel_cb);
#endif
  }
}

void doPart(const struct message msg, struct connection *conn) {
  if (msg.type == MSG_PART) {
    char *channel = getChannel(msg.argv[0]);
    tdelete(channel, (void **)&(conn->channels_root), strcmp_void);
  }
}
