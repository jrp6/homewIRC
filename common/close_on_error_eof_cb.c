#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>

#include "close_on_error_eof_cb.h"

void close_on_error_eof_cb(struct bufferevent *bev, short events, void *ptr)
{
  (void)ptr;
  if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
    if (events & BEV_EVENT_ERROR) {
      int err = bufferevent_socket_get_dns_error(bev);
      if (err) {
        printf("DNS Error: %s\n", evutil_gai_strerror(err));
      }
    }
    bufferevent_free(bev);
  }
}
