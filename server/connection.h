#ifndef _CONNECTION_H
#define _CONNECTION_H

struct connection {
  char *nick;
  char *hostmask;
};

void freeConnection(struct connection conn);

#endif
