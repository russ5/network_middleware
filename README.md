# network_middleware
### Created by: Tae Yoon Choi, Ryan Dumouchel, and Russell Lamey

This is a C library to make the process of creating a network of connected computers using TCP/IP much simpler.

## How to use this library

To use any of the 3 existing topologies, create a config file containing the IP addresses and port numbers of all the computers in the network you wish to create.

Once this is done, compile and run the \_background.c application on all machines. Then create your application, and call one of the Setup() functions, making sure to pass in the location of your config file and the location of the application you wish to run on all machines. Then compile and run your application on your desired "master" computer.

After setup, communicate between the nodes using Send() and Receive() as desired. To see an example of this, navigate to the /\_final/test folder to see a demonstration.
