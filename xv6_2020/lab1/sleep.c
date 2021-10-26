#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"





int
main(int argc, char *argv[])
{
  int num;

  if(argc < 2){
    fprintf(2, "Usage: sleep num...\n");
    exit(1);
  }

  num = atoi(argv[1]);
//   printf("num is %d\n",num);
  sleep(num);

  exit(0);
}
