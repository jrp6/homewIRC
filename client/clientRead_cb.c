#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>

#include "clientRead_cb.h"
#include "defaults.h"
#include "logMessage.h"
#include "message.h"
#include "replyPing.h"

void clientRead_cb(struct bufferevent *bev, void *ctx) {
  (void)ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    #ifdef CLIENT_DEBUG
    puts(buf);
    #endif

    struct message msg = parseMessage(buf);
    logMessage(msg);

    switch (msg.type) {
    case MSG_PING:
      replyPing(&msg, bev);
      break;
    case MSG_NICK:
    case MSG_USER:
    case MSG_PONG:
    case MSG_PRIVMSG:
    case MSG_JOIN:
    case MSG_PART:
    case MSG_QUIT:
    case MSG_INVALID:
      break;
    }

    freeMessage(msg);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }

  evbuffer_free(evbuf);
}
