#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "defaults.h"
#include "message.h"

#ifdef SERVER_DEBUG
#include <stdio.h>
#include "logMessage.h"
#endif

void serverRead_cb(struct bufferevent *bev, void *ctx) {
  struct connection *conn = ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    struct message msg = parseMessage(buf);
#ifdef SERVER_DEBUG
    logMessage(msg);
#endif

    switch (msg.type) {
    case MSG_NICK: {
      char *nick = msg.argv[0];
#ifdef SERVER_DEBUG
      if (conn->nick) {
        printf("Conn %u: Changing nick %s -> %s\n", conn->id, conn->nick, nick);
      } else {
        printf("Conn %u: Setting initial nick %s\n", conn->id, nick);
      }
#endif
      free(conn->nick);
      conn->nick = malloc((strlen(nick) + 1) * sizeof(char));
      strcpy(conn->nick, nick);
      break;
    }
    case MSG_USER:
      // TODO: Change hostmask
    case MSG_PING:
    case MSG_PONG:
    case MSG_PRIVMSG:
    case MSG_JOIN:
    case MSG_PART:
    case MSG_QUIT:
    case MSG_CMD_CHANNEL:
    case MSG_INVALID:
      break;
    }

    freeMessage(msg);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }
  free(evbuf);
}
