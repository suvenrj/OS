#include "types.h"
#include "stat.h"
#include "user.h"

char buf[1];

void
cat(int fd, int num_lines)
{
  int n;

  while((n = read(fd, buf, sizeof(buf))) > 0) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }
    if (buf[0]=='\n'){
        num_lines--;
    }
    if (!num_lines){
        break;
    }
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int num_lines = atoi(argv[1]);
  if(argc <= 2){
    cat(0, num_lines);
    exit();
  }
  for(i = 2; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    cat(fd, num_lines);
    close(fd);
  }
  exit();
}
