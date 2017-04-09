#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <event2/bufferevent.h>

struct connection {
  unsigned int id;
  struct bufferevent *bev;
  char *nick;
  char *hostmask;
};

void removeConnection(const unsigned int id);
struct connection * newConnection(struct bufferevent *bev);
struct connection * getConnection(const unsigned int id);
unsigned int getNConns(void);

#endif
