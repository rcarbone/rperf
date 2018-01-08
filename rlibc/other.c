/*
 * other.c - Miscellanea
 */


/* System headers */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/utsname.h>


/* System headers */
#include "support.h"


/* Return the full qualified hostname */
char * rfqname (void)
{
  struct utsname machine;
  struct sockaddr_in in;

  uname (& machine);

  /* Attempt to resolv hostname to get the internet address */
  struct hostent * h = gethostbyname (machine . nodename);

  if (h)
    memcpy (& in . sin_addr, h -> h_addr_list [0], h -> h_length);
  else
    in . sin_addr . s_addr = inet_addr (machine . nodename);

  /* Back to the full qualified domain address */
  h = gethostbyaddr ((char *) & in . sin_addr, sizeof (struct in_addr), AF_INET);

  return ! h || ! h -> h_name ? inet_ntoa (in . sin_addr) : h -> h_name;
}


/* Announce to the world! */
void rhelloworld (char * progname, char * version, char * date, char * _time, char * nodename, char * author)
{
  time_t now = time (0);
  char * nowstring = ctime (& now);

  printf ("This is %s %s (%s %s)\n", progname, version, date, _time);
  printf ("Started %24.24s on %s\n", nowstring, nodename);
  printf ("By %s\n", author);
  printf ("\n");
  fflush (stdout);
}


/* Display version information */
void rversion (char * progname, char * version, char * date, char * time)
{
  printf ("%s version %s built on %s %s\n", progname, version, date, time);
  fflush (stdout);
}
