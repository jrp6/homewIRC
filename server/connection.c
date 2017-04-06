#include <stdlib.h>
#include "connection.h"

void freeConnection(struct connection conn)
{
  free(conn.nick);
  free(conn.hostmask);
}
