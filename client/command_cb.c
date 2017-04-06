#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>

#include "command_cb.h"

void command_cb(struct bufferevent *bev, void *ctx)
{
  (void)ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    puts(buf);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }

  evbuffer_free(evbuf);
}
