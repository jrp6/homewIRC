#include <event2/bufferevent.h>
#include <event2/event.h>

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "nick_on_connect_cb.h"
#include "defaults.h"
#include "echo_cb.h"

// TODO: Remove these after removing the ugly hack
#include "message.h"
#include <stdlib.h>

int main(void)
{
  // Ugly hack for testing if message parsing and stringifying works. TODO: unit tests
  struct message test = parseMessage("NICK juhorp");
  char *str = stringifyMessage(test);
  puts(str);
  free(str);
  freeMessage(test);

  struct event_base *base;
  struct bufferevent *bev;
  struct sockaddr_in sin;

  base = event_base_new();
  bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

  bufferevent_setcb(bev, echo_cb, NULL, nick_on_connect_cb, base);
  bufferevent_enable(bev, EV_READ|EV_WRITE);

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(0x7f000001); // 127.0.0.1
  sin.sin_port = htons(DEFAULT_PORT);

  if (bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
    puts("FATAL: Could not open socket.");
    bufferevent_free(bev);
    return -1;
  }

  event_base_dispatch(base);

  return 0;
}
