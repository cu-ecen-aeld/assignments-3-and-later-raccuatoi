#include <stdio.h>
#include <syslog.h>
int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments.\n");
    syslog(LOG_ERR, "Error! Number of arguments must be 3.");
    return 1;
  }
  FILE *fp;
  fp = fopen(argv[1], "w");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open the file %s.\n", argv[1]);
    syslog(LOG_ERR, "Error opening  %s.\n", argv[1]);
    return 1;
  }
  fprintf(fp, "%s", argv[2]);
  syslog(LOG_DEBUG, "Writing %s", argv[2]);
  fclose(fp);
  return 0;
}

