#include <unistd.h>

int main(void){
  char* target = "../bin/cc";
  char* link = "demacc";

  int res;

  res = symlink(target, link);

  return res;
}
