#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <string.h>

#include "close_on_error_eof_cb.h"
#include "nick_on_connect_cb.h"

void nick_on_connect_cb(struct bufferevent *bev, short events, void *ptr)
{
  if (events & BEV_EVENT_CONNECTED) {
    const char *msg = "NICK juhorp\r\nUSER juhorp 0 * :Unknown\r\n";
    bufferevent_write(bev, msg, strlen(msg));
  } else if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
    close_on_error_eof_cb(bev, events, ptr);
  }
}
