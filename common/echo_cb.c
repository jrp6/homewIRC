#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <stdio.h>
#include <stdlib.h>

#include "echo_cb.h"

#define BUFSIZE 1024

void echo_cb(struct bufferevent *bev, void *ctx) {
  (void)ctx;

  char *buf = malloc(BUFSIZE * sizeof(char));
  size_t n = bufferevent_read(bev, buf, BUFSIZE * sizeof(char) - 1);
  buf[n] = '\0';
  fputs(buf, stdout);
  free(buf);
}
