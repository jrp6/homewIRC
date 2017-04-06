#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_cb.h"
#include "message.h"

static char currentChannel[128] = "";

void command_cb(struct bufferevent *bev_stdin, void *ctx)
{
  struct bufferevent *bev = ctx;

  struct evbuffer *evbuf = evbuffer_new();
  bufferevent_read_buffer(bev_stdin, evbuf);

  char *buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);

  while (buf) {
    struct message msg;
    if (*buf == '/') {
      msg = parseMessage(buf + 1);
      if (msg.type == MSG_CMD_CHANNEL) {
        strncpy(currentChannel, msg.argv[0], 128);
      } else if (msg.type != MSG_INVALID) {
        // Send the command straight to the server if it is valid and not CHANNEL
        if (msg.type == MSG_JOIN) {
          char *channel = msg.argv[msg.argc - 1];
          if (*channel != ':') {
            strncpy(currentChannel, msg.argv[msg.argc - 1], 128);
          } else {
            strncpy(currentChannel, msg.argv[msg.argc - 1] + 1, 128);
          }
        }
      } else {
        puts("Invalid command");
      }
    } else {
      // Send the input as a PRIVMSG to the current channel
      msg.source = NULL;
      msg.type = MSG_PRIVMSG;
      msg.argv = malloc(2 * sizeof(char *));
      msg.argv[0] = malloc((strlen(currentChannel) + 1) * sizeof(char));
      strcpy(msg.argv[0], currentChannel);
      msg.argv[1] = malloc((strlen(buf) + 2) * sizeof(char));
      msg.argv[1][0] = ':';
      strcpy(msg.argv[1] + 1, buf);
      msg.argc = 2;
    }

    if (!isLocal(msg.type)) {
      char *str = stringifyMessage(msg);
      bufferevent_write(bev, str, strlen(str));
      free(str);
    }

    freeMessage(msg);
    free(buf);
    buf = evbuffer_readln(evbuf, NULL, EVBUFFER_EOL_ANY);
  }

  evbuffer_free(evbuf);
}
