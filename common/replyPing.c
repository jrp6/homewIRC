#include <event2/bufferevent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defaults.h"
#include "message.h"
#include "replyPing.h"

void replyPing(const struct message *ping, struct bufferevent *bev)
{
  if (ping->type != MSG_PING) {
    fputs("replyPing: invalid message type\n", stderr);
    return;
  }

  size_t n = sizeof(*ping);
  struct message *pong = malloc(n);
  memcpy(pong, ping, n);
  pong->type = MSG_PONG;
  pong->source = NULL;

  char *str = stringifyMessage(*pong);
  #ifdef CLIENT_DEBUG
  printf("Sending PONG msg %s", str);
  #endif
  bufferevent_write(bev, str, strlen(str));

  free(str);
  // Do not call freeMessage(pong) because the pointers are still referenced by ping
  free(pong);
}
