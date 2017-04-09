#include <event2/bufferevent.h>
#include <string.h>

#include "connection.h"
#include "message.h"
#include "transmitPrivmsg.h"

void transmitPrivmsg(const struct message msg, const struct connection *conn)
{
  if (msg.type != MSG_PRIVMSG) {
    return;
  }

  struct message transmitMsg = msg;
  transmitMsg.source = conn->hostmask;

  char *transmitStr = stringifyMessage(transmitMsg);

  // Iterate through all connections and send transmitStr to them
  struct connection *conn_l;
  for (unsigned int i = 0; i < getNConns(); ++i) {
    if ((conn_l = getConnection(i))) {
      bufferevent_write(conn_l->bev, transmitStr, strlen(transmitStr));
    }
  }
}
