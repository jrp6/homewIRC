#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_cb.h"
#include "message.h"

void command_cb(struct bufferevent *bev_stdin, void *ctx)
{
  struct bufferevent *bev = ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev_stdin, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    if (*buf == '/') {
      // Send the command straight to the server if it is valid
      struct message msg = parseMessage(buf + 1);
      if (msg.type != MSG_INVALID) {
        char *str = stringifyMessage(msg);
        bufferevent_write(bev, str, strlen(str));
        free(str);
      } else {
        puts("Invalid command");
      }
      freeMessage(msg);
    } else {
      puts("TODO: Handle sending messages");
    }

    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }

  evbuffer_free(evbuf);
}
