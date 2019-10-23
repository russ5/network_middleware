/*
 * tcp_client.c
 *
 * The program in this file is based on the example in Figure 7a from
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
 * and attempts to initiate a TCP connection for reliable communication
 * to a socket endpoint at a specified port on another host computer 
 * (which could be the local host, if desired). If the connection is
 * established successfully, a single text string is transmitted,
 * and the local socket is closed, which triggers the termination of
 * the connection (i.e., a terminate message is sent to the other end).
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

/*
 * In case of any error, this program exits with the same return value of -1.
 * (The original program used different positive integers as program
 * return values for different error cases.)
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#define TEXT_TO_SEND "Hello from the other end of the connection ..."

int main (int argc, char *argv[])
{
  /* VARIABLES */

  int sock;	/* this is effectively an identifier to an open 'file'
		   because of the UNIX-style common handling of files
		   and local/non-local communication streams;
		   when set by a socket() call, it will identify a
		   socket endpoint rather than a normal file */

  struct hostent *hp;	/* this data structure is used when resolving
			   the identification of the host to which the
			   connection will be made; the code below uses
			   gethostbyname() to obtain the IP address of
			   a host, and the information is returned as a
			   pointer to a structure of this type */
			   
  struct sockaddr_in server;	/* this data structure is a specialized
				   version of the sockaddr structure;
				   '_in' is for Internet-oriented sockets;
				   it is used to prepare the specific
				   identification for the other endpoint
				   of the connection request */

  int bytes_to_send; /* the desired number of bytes to send */

  int count; /* the number of bytes actually written for data transmission */

  /* -------------------------------------------------------------------- */

  /* Create a socket on this host, requesting that it be for Internet (IPv4),
     that it use reliable data streaming for the communication, and
     that it specifically use the TCP protocol for this purpose. */

  sock = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    perror ("opening stream socket");
    exit (-1);
  }

  /* Perform a name lookup using the second argument from the command line. */

  hp = gethostbyname (argv[1]);
  if (hp == 0) {
    fprintf (stderr, "%s: unknown host", argv[1]);
    exit (-1);
  }

  /* Prepare the structure to be used for requesting the connection. The
     address family to use is set to IPv4 adddresses. The 32-bit address
     resulting from the by-name lookup is copied to the appropriate field.
     The port to use at the other host for the connection attempt is set
     from the second command-line argument. The function htons() performs
     any necessary translation between network-standard byte ordering and
     computer-specific byte ordering. If the call to the connect() routine
     is successful, then the socket identified by 'sock' is ready to use
     for sending (and more generally also for receiving) data. There is a
     type cast for the second argument for connect() in the code below
     to eliminate a compiler warning arising from connect() expecting a
     pointer to the more generic sockaddr structure, but this program
     using the more specialized Internet-oriented sockaddr_in structure. */

  server.sin_family = AF_INET;
  bcopy (hp->h_addr, &server.sin_addr, hp->h_length);
  server.sin_port = htons (atoi (argv[2]));
  if (connect (sock, (struct sockaddr *) &server, sizeof (server)) < 0) {
    perror ("connecting stream socket");
    exit (-1);
  }

  /* Perform a single write (treating the socket effectively as a file)
     with the predefined text string as the data to send to the other host.
     The write() call returns the number of bytes successfully written
     (which in this case means bytes successfully accepted for transmission.)
     In principle, code must be prepared in a manner that handles cases where
     the actual number of bytes written is less than the total specified.
     The return count can be used as an offset from the original start of
     the full byte sequence to attempt another write with the portion that
     was not written in the first attempt (recursively pursuing further
     attempts of on each subsequence if less than the specified number of
     bytes are written in each attempt). In this example, however, it is
     assumed that bytes are successfully written in one attempt, which
     should be the normal occurrence. Output is printed to indicate
     whether or not all bytes were accepted in the single attempt. */

  bytes_to_send = sizeof (TEXT_TO_SEND);  /* includes '\0' at end of string */
  count = write (sock, TEXT_TO_SEND, bytes_to_send);
  if (count < 0)
    perror ("writing on stream socket");
  else
  {
    if (count == bytes_to_send)
      printf ("all bytes sent successfully\n");
    else
      printf ("sent only %d out of %d bytes\n", count, bytes_to_send);
  }

  /* For this sample program, there is no other transmission to perform
     (in either direction). Hence, the socket on the local host is closed
     using the same low-level library routine that would be used for files. */

  close (sock);

  /* The consequence of the above call with respect to the TCP protocol
     is the sending of a termination message to the other end of the
     communication, causing appropriate action at the other end. */

  return 0;	/* This program terminates with a 'success' return value. */
}
