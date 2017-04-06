#ifndef _CONNECTION_H
#define _CONNECTION_H

struct connection {
  unsigned int id;
  char *nick;
  char *hostmask;
};

void removeConnection(const unsigned int id);
struct connection * newConnection(void);
struct connection * getConnection(const unsigned int id);
unsigned int getNConns(void);

#endif
