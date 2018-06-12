# Socket-GET-request
Connects to a server on a TCP socket and logs the server's response to an HTTP GET request that you send it via the socket.
This program creates a socket and makes a GET request.
## Whats here
- option_handler.h (An argumnet handeling tool from: https://github.com/ryngonzalez/OptionHandler)
- knock.cpp (Program use to create socket and make GET request)
## Setup
To start clone this repo:
```
git clone https://github.com/SeanFitz1997/Socket-GET-request.git
```
Then to compile the files enter
```
make
```
If there is and issues enter
```
make clean && make
```
## Example 
(GET request)
```
./knock -h www.tcd.ie -p 80 -f output.txt
```
This will send an HTTP GET request to the webpage _[www.tcd.ie](http://www.tcd.ie)_ at the socket listening at port 80 and will record the response to the file _[output.txt]_.
(Usage)
```
./knock -?
or
./knock -H
```
## Parameters 
Required:
* **h, host:** the DNS name or IP address of the host to contact
* **p, port:** the TCP port on the server to which to connect
Optional:
* **w, web:** make an HTTP GET request for the "/" resource
* **f, file:** store any output received from socket in file 
* **H, help:** produce a usage message such as that below
* **?, help:** produce a usage message such as that below

## Important
* The host input MUST be an IPv4 or IPv6 address or a Fully Qualified Domain Name (FQDN)
* The port MUST be an integer between 1 and 65535
* The mandatory parameters are the host and port.
