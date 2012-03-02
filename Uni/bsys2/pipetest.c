/* Pipe-Test 
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

int main () {
  mode_t mode;
  int rc;
  char fname[100];
  struct stat buf;

  puts ("Pipe-Test");
  printf ("Pipename: ");
  gets (fname);
  rc = stat (fname, &buf);
  if (rc < 0) {
    if (errno != ENOENT) {
      perror("stat");
      exit (1);
    }
    puts ("Pipe existiert noch nicht");
    mode = 0644;
    rc = mkfifo (fname, mode);
    if (rc < 0) {
      perror("mkfifo");
      exit (2);
    } else {
      printf ("Pipe %s wurde angelegt\n", fname);
    }
  } else {
    if ( ! S_ISFIFO(buf.st_mode)) {
      printf ("File %s existiert, ist keine Pipe!\n", fname);
      exit (3);
    }
    printf ("Pipe %s existiert bereits\n", fname);
  }
  exit (0);
} 
