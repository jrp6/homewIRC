#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/event.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "nick_on_connect_cb.h"
#include "defaults.h"
#include "echo_cb.h"

// TODO: Remove these after removing the ugly hack
#include "message.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
  // Ugly hack for testing if message parsing and stringifying works. TODO: unit tests
  struct message test = parseMessage("NICK juhorp");
  char *str = stringifyMessage(test);
  puts(str);
  free(str);
  freeMessage(test);

  // Parse command-line options
  char *server = NULL;
  unsigned int port = DEFAULT_PORT;
  const char *optstring = "s:p:";
  int c;
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch (c) {
    case 's':
      server = optarg;
      break;
    case 'p':
      port = atoi(optarg);
      break;
    default:
      exit(EXIT_FAILURE);
    }
  }

  if (!server) {
    exit(EXIT_FAILURE);
  }

  // Connect and start reading and writing
  struct event_base *base;
  struct bufferevent *bev;
  struct evdns_base *dns_base;

  base = event_base_new();
  dns_base = evdns_base_new(base, 1);

  bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);

  bufferevent_setcb(bev, echo_cb, NULL, nick_on_connect_cb, base);
  bufferevent_enable(bev, EV_READ|EV_WRITE);

  if (bufferevent_socket_connect_hostname(bev, dns_base, AF_UNSPEC, server, port) < 0) {
    puts("FATAL: Could not open socket.");
    bufferevent_free(bev);
    exit(EXIT_FAILURE);
  }

  event_base_dispatch(base);

  exit(EXIT_SUCCESS);
}
