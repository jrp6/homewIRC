#include <event2/bufferevent.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "message.h"

void sendWelcome(struct connection *conn, struct bufferevent *bev)
{
  const char *welcomeStr = ":Welcome to the Internet Relay Network ";
  struct message msg;
  msg.source = NULL;
  msg.type = MSG_RPL_WELCOME;
  msg.argc = 2;
  msg.argv = malloc(2 * sizeof(char *));
  msg.argv[0] = conn->nick;

  const size_t n = strlen(welcomeStr) + strlen(conn->hostmask) + 1;
  msg.argv[1] = calloc(n, sizeof(char));
  strcpy(msg.argv[1], welcomeStr);
  strcat(msg.argv[1], conn->hostmask);

  char *msgstr = stringifyMessage(msg);
  bufferevent_write(bev, msgstr, strlen(msgstr));

  free(msg.argv[1]);
  free(msgstr);
}
