#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdlib.h>

#include "defaults.h"
#include "message.h"

#ifdef SERVER_DEBUG
#include <stdio.h>
#include "logMessage.h"
#endif

void serverRead_cb(struct bufferevent *bev, void *ctx) {
  (void)ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    #ifdef SERVER_DEBUG
    puts(buf);
    #endif
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }
}
