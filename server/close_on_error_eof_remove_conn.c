#include <event2/bufferevent.h>
#include <stdlib.h>
#include "connection.h"
#include "close_on_error_eof_cb.h"

void close_on_error_eof_remove_conn(struct bufferevent *bev, short events, void *ptr)
{
  if (events & (BEV_EVENT_ERROR|BEV_EVENT_EOF)) {
    struct connection *conn = ptr;
    removeConnection(conn->id);
    free(conn);
    close_on_error_eof_cb(bev, events, ptr);
  }
}
