#include <event2/bufferevent.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "message.h"
#include "strcmp_void.h"
#include "transmitPrivmsg.h"

void transmitPrivmsg(const struct message msg, const struct connection *conn)
{
  if (msg.type != MSG_PRIVMSG && msg.type != MSG_JOIN && msg.type != MSG_PART) {
    return;
  }

  struct message transmitMsg = msg;
  transmitMsg.source = conn->hostmask;

  char *transmitStr = stringifyMessage(transmitMsg);

  // Iterate through all connections and send transmitStr to them if necessary
  struct connection *conn_l;
  for (unsigned int i = 0; i < getNConns(); ++i) {
    if ((conn_l = getConnection(i))) {
      if (conn_l != conn &&
          (strcmp(conn_l->nick, msg.argv[0]) == 0 ||
           tfind(msg.argv[0], (void **)&(conn_l->channels_root), strcmp_void))) {
        bufferevent_write(conn_l->bev, transmitStr, strlen(transmitStr));
      }
    }
  }
  free(transmitStr);
}
