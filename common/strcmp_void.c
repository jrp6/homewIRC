#include <string.h>

int strcmp_void(const void *a, const void *b)
{
  const char *ac = a;
  const char *bc = b;
  return strcmp(ac, bc);
}
