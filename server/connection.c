#define _GNU_SOURCE
#include <event2/bufferevent.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include <search.h>

#include "connection.h"

// N.B. For good performance, ensure that your C library implements tsearch etc using balanced trees

static void freeConnection(struct connection conn)
{
  free(conn.nick);
  free(conn.hostmask);
  tdestroy(conn.channels_root, free);
}

static int compareConnections(const void *leftv, const void *rightv)
{
  const struct connection *left = leftv;
  const struct connection *right = rightv;
  return left->id - right->id;
}

static struct connection * dummyConn(unsigned int id)
{
  struct connection *conn = malloc(sizeof(struct connection));
  conn->id = id;
  conn->nick = NULL;
  conn->hostmask = NULL;
  conn->channels_root = NULL;
  return conn;
}

void *root = NULL;
unsigned int nConns = 0;

void removeConnection(const unsigned int id)
{
  struct connection *toRemove = getConnection(id);
  if (toRemove) {
    freeConnection(*toRemove);
    tdelete(toRemove, &root, compareConnections);
  }
}

struct connection * newConnection(struct bufferevent *bev)
{
  struct connection *c = dummyConn(nConns++);
  c->bev = bev;
  struct connection **ct = tsearch(c, &root, compareConnections);
  return *ct;
}

struct connection * getConnection(const unsigned int id)
{
  struct connection *c = dummyConn(id);
  struct connection **ct = tfind(c, &root, compareConnections);
  free(c);
  return ct ? *ct : NULL;
}

unsigned int getNConns(void)
{
  return nConns;
}
