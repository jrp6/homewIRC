#include <search.h>
#include <stdlib.h>
#include "connection.h"

//N.B. For good performance, ensure that your C library implements tsearch etc using balanced trees

static void freeConnection(struct connection conn)
{
  free(conn.nick);
  free(conn.hostmask);
}

static int compareConnections(const void *leftv, const void *rightv)
{
  const struct connection *left = leftv;
  const struct connection *right = rightv;
  return left->id - right->id;
}

static struct connection dummyConn(unsigned int id)
{
  struct connection conn;
  conn.id = id;
  conn.nick = NULL;
  conn.hostmask = NULL;
  return conn;
}

void *root = NULL;
unsigned int nConns = 0;

void removeConnection(const unsigned int id)
{
  struct connection *toRemove = getConnection(id);
  if (toRemove) {
    freeConnection(*toRemove);
    struct connection c = dummyConn(id);
    tdelete(&c, &root, compareConnections);
  }
}

struct connection * newConnection(void)
{
  struct connection c = dummyConn(nConns++);
  return (struct connection *)tsearch(&c, &root, compareConnections);
}

struct connection * getConnection(const unsigned int id)
{
  struct connection c = dummyConn(id);
  return tfind(&c, &root, compareConnections);
}

unsigned int getNConns(void)
{
  return nConns;
}
