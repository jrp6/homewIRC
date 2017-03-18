#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "accept_cb.h"
#include "close_on_error_eof_cb.h"
#include "echo_cb.h"

void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd,
                           struct sockaddr *address, int socklen, void *ctx) {
  (void)address;
  (void)socklen;
  (void)ctx;

  // Setup bufferevent for the new connection
  struct event_base *base = evconnlistener_get_base(listener);
  struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

  // Add callback that prints everything we get
  bufferevent_setcb(bev, echo_cb, NULL, close_on_error_eof_cb, NULL);

  bufferevent_enable(bev, EV_READ|EV_WRITE);

  // Write "Hello World" for debugging
  const char *hello = "Hello World!\n";
  bufferevent_write(bev, hello, strlen(hello));
}

void accept_error_cb(struct evconnlistener *listener, void *ctx) {
  (void)ctx;

  struct event_base *base = evconnlistener_get_base(listener);
  int err = EVUTIL_SOCKET_ERROR();
  fprintf(stderr, "Got error %d %s on listener. Shutting down.\n", err, evutil_socket_error_to_string(err));
  event_base_loopexit(base, NULL);
}
