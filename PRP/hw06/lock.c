#include "lock.h"

#include <string.h>

static const char *ORIGINAL_PASSWORD = "bac";

bool unlock(char *password){
  return !strcmp(password, ORIGINAL_PASSWORD);
}
