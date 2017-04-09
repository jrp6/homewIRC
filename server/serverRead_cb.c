#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <stdlib.h>
#include <string.h>

#include "broadcastMsg.h"
#include "connection.h"
#include "defaults.h"
#include "doJoin.h"
#include "hostmask_util.h"
#include "message.h"
#include "transmitPrivmsg.h"
#include "replyPing.h"
#include "sendWelcome.h"

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
      broadcastMsg(msg, conn);
      free(conn->nick);
      conn->nick = malloc((strlen(nick) + 1) * sizeof(char));
      strcpy(conn->nick, nick);
      if (conn->hostmask) {
        changeNick(nick, &(conn->hostmask));
#ifdef SERVER_DEBUG
        printf("Conn %u: Changed hostmask to %s\n", conn->id, conn->hostmask);
#endif
      }
      break;
    }
    case MSG_USER: {
      char *user = msg.argv[0];
      char *host = "unknown-host";
      conn->hostmask = getHostmask(conn->nick, user, host);
#ifdef SERVER_DEBUG
      printf("Conn %u: Setting hostmask %s\n", conn->id, conn->hostmask);
#endif
      sendWelcome(conn, bev);
      break;
    }
    case MSG_PRIVMSG:
      transmitPrivmsg(msg, conn);
      break;
    case MSG_PING:
      replyPing(&msg, bev);
      break;
    case MSG_JOIN:
      doJoin(msg, conn);
      break;
    case MSG_PART:
      doPart(msg, conn);
      break;
    case MSG_QUIT:
    case MSG_PONG:
    case MSG_CMD_CHANNEL:
    case MSG_RPL_WELCOME:
    case MSG_INVALID:
      break;
    }

    freeMessage(msg);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }
  free(evbuf);
}
