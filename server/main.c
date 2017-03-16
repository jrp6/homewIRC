#include <event2/listener.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <string.h>

#include "accept_cb.h"
#include "defaults.h"

int main(void)
{
  struct event_base *base;
  struct evconnlistener *listener;
  struct sockaddr_in sin;

  base = event_base_new();
  if (!base) {
    puts("FATAL: Couldn't create event base");
    return 1;
  }

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = htonl(0); // Listen on 0.0.0.0
  sin.sin_port = htons(DEFAULT_PORT);

  listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
                                     -1, (struct sockaddr*)&sin, sizeof(sin));
  if (!listener) {
    perror("FATAL: Couldn't create listener");
    return 1;
  }
  evconnlistener_set_error_cb(listener, accept_error_cb);

  event_base_dispatch(base);

  return 0;
}
