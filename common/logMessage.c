#include <stdio.h>
#include <stdlib.h>

#include "logMessage.h"
#include "message.h"

void logMessage(const struct message msg)
{
  char *msgstr = stringifyMessage(msg);
  fputs(msgstr, stdout);
  free(msgstr);
}
