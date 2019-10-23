/*
 * tcp_server.c
 *
 * The program in this file is based on the example in Figure 7b from
 * the following document:
 *
 *    Stuart Sechrest, "An Introductory 4.4BSD Interprocess Communication
 *    Tutorial," Document PSD:20, FreeBSD documentation archive at
 *    https://docs.freebsd.org/44doc
 *
 * The above document is based on a 1986 technical report by the same
 * author at the University of California - Berkeley.
 * 
 * This file is an adaptation of the original program with some minor
 * modifications and elaborated comments by Dr. Naraig Manjikian, Dept.
 * of Electrical and Computer Engineering, Queen's University, for the
 * benefit of students enrolled in ELEC 373 Computer Networks.
 */

/* This program creates a socket endpoint on the local host computer,
 * and awaits TCP connection attempts on the appropriate port. Those
 * connection attempts could come from an altogether different host
 * or the same local host. Once a connection is established, this
 * server program assumes that the client will send a stream of bytes
 * (presumably, null-terminated character strings). Nothing is sent back
 * by the server to the client (although that could certainly occur in
 * a different application). The server repeatedly reads incoming bytes
 * on the local socket endpoint until a return value of 0, which is an
 * indication that the connection has been terminated (typically as a
 * direct action by the client, but possibly also due to some error).
 * The socket used for the connection is closed, and the server program
 * again awaits a connection request. In its present form, this program
 * executes forever, and requires explicit termination by the user.
 */

/*
 * The code below uses the standard C perror() library routine that enables
 * meaningful error messages to be printed to the stderr output file. Other
 * library routines set a unique internal error code if something goes wrong,
 * and they also return a more generic error indication (e.g., -1 value)
 * to user-written calling code. If the user-written code immediately calls
 * perror(), then the most recent unique internal code is converted to a
 * human-readable text string that is printed to stderr. The string passed
 * to perror() is printed before the specific error message; in this manner,
 * the user-written code can identify where the error occurred.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#define BUF_SIZE 1024

/*
 * This program creates a socket and then begins an infinite loop. Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 */
int main (int argc, char *argv[])
{
  /* VARIABLES */

  int sock, msgsock;
		 /* these are effectively identifiers to an open 'file'
		   because of the UNIX-style common handling of files
		   and local/non-local communication streams;
		   when set by a socket() or accept(), they identify a
		   socket endpoint rather than a normal file;
		   in this program, sock is for the listening socket,
		   and msgsock is the one for an accepted connection */

  struct sockaddr_in server;	/* this data structure is a specialized
				   version of the sockaddr structure;
				   '_in' is for Internet-oriented sockets;
				   it is used to prepare the specific
				   identification for the endpoint for
				   listing to connection requests */

  struct sockaddr_in client;	/* same comment as above, but used to
				   identify the client endpoint after
				   accepting a connection request */

  int length;	/* used as argument to and from library routine for
		   size of data structure with IP address information */

  in_addr_t ip_address;	/* an IPv4 address is 32 bits, and the header files
			   above define an unsigned integer type of the
			   appropriate size for this purpose; this variable
			   is used to obtain the IP address of the client
			   after a connection request is accepted */

  char buf[BUF_SIZE];	/* space for incoming byte stream from client */

  int count;	/* number of bytes actually read from socket */

  /* -------------------------------------------------------------------- */

  /* Create a socket on this host, requesting that it be for Internet (IPv4),
     that it use reliable data streaming for the communication, and
     that it specifically use the TCP protocol for this purpose. */

  sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    perror ("opening stream socket");
    exit (-1);
  }

  /* Prepare the structure for identifying the server locally and associate it
     (i.e., bind it) with the above socket that will be used to listen for
     connection attempts. A wildcard is used for the local IP address because
     this server program does not require knowledge of the local IP address
     (it is actually the client software that must know the server IP address).
     For the port, a specific (presumably unused) identifier is specified;
     providing a 0 for the port would be treated as a wildcard, and in this
     case the system must assign an available port identifier. Any client
     software would somehow need to be informed of a system-selected port.
     For this example, a fixed predetermined value of 59000 is used. In this
     manner, both endpoints agree on the port information for connecting. */

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons (59000);
  if (bind (sock, (struct sockaddr *) &server, sizeof (server)))
  {
    perror ("binding stream socket");
    exit (-1);
  }

  /* Confirm port information after binding to socket (if a zero port had been
     specified, the system-assigned port number would be obtained this way). */

  length = sizeof (server);
  if (getsockname (sock, (struct sockaddr *) &server, &length))
  {
    perror ("getting socket name");
    exit (-1);
  }
  printf ("Listening socket uses port %d\n", ntohs (server.sin_port));

  /* Instruct system to prepare for connection attempts on listening socket,
     with a backlog limit (i.e., how many requests from multiple sources)
     set to 5. (Systems can enforce their own limit on the backlog.) */

  listen (sock, 5);

  /* Enter an infinite loop that begins with an accept() call that blocks
     until a connection attempt is received. The arguments to accept()
     allow subsequent code in this program to obtain the client IP address. */

  for (;;)
  {
    length = sizeof (client);
    msgsock = accept (sock, (struct sockaddr *) &client, &length);
    if (msgsock == -1)
      perror ("accept");
    else
    {
      /* A connection has been accepted. Identify the client side. */

      ip_address = ntohl (client.sin_addr.s_addr);
      printf ("Client socket uses IP address %d.%d.%d.%d\n",
	      (ip_address >> 24) & 0xFF, (ip_address >> 16) & 0xFF,
	      (ip_address >>  8) & 0xFF, (ip_address >>  0) & 0xFF);
      printf ("Client socket uses port %d\n", ntohs (client.sin_port));

      /* The loop below is executed as long as the connection has not been
	 terminated (indicated by a return value of zero for reading data). */

      do
      {
	count = read (msgsock, buf, BUF_SIZE);

	if (count == 0)
	{
	  printf ("Connection is being terminated.\n");
	}
	else if (count < 0)
	{
	  perror ("reading stream message");
	}
	else
	{
	  /* Because a string is to be printed, ensure that there is a
	     null terminator at the position corresponding to the last byte
	     that was received. If not, simply put one there for safety.
	     In principle, it is possible for byte streams to be received
	     in portions that do not necessarily reflect the application-level
	     logical grouping of data. Proper code for reading bytes must
	     handle the possibility of such fragmentation. For this example
	     program, the simple step of forcing the introducing of a
	     a null terminator is used as a safety measure (which could
	     possibly overwrite a valid byte in the original stream). */

	  if (buf[count - 1] != '\0')
	    buf[count - 1] = '\0';

	  printf ("Received from client:\n\t%s\n", buf);
	}
      } while (count != 0);

      /* If the connection has been terminated, close the socket that was
	 assigned following the competion of the accept() call. */

      close (msgsock);
    }
  }

  /* With an infinite loop above, the listening socket is never closed
     by code in this program. A user must terminate the program itself.
     Fortunately, the operating system support normally performs any
     "clean-up" after a program is explicitly terminated by a user,
     hence any program with active sockets will have those sockets
     automatically closed by the system. */

  return 0;	/* never reached, but needed to eliminate compiler message */
}
