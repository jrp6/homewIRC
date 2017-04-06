#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>

#include "echo_cb.h"
#include "logMessage.h"
#include "message.h"

void echo_cb(struct bufferevent *bev, void *ctx) {
  (void)ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    puts(buf);

    struct message msg = parseMessage(buf);
    logMessage(msg);
    freeMessage(msg);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }
}
