#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "close_on_error_eof_cb.h"
#include "nick_on_connect_cb.h"

void nick_on_connect_cb(struct bufferevent *bev, short events, void *ptr)
{
  if (events & BEV_EVENT_CONNECTED) {
    char *nick = ptr;
    char *msg = malloc(256 * sizeof(char));
    snprintf(msg, 256, "NICK %s\r\nUSER %s 0 * :Unknown\r\n", nick, nick);
    bufferevent_write(bev, msg, strlen(msg));
  } else if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
    close_on_error_eof_cb(bev, events, ptr);
  }
}
