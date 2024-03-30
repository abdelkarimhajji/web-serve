The server part :
    1- create a socket
    2- bind the socket : associate a socket with a specific local address and port
    3- put the socket in the listening state
    4- starting from this point, i keep monitoring -using select- select() works by blocking until something happens on a file descriptor (aka a socket)
    5- if some fd is ready for reading, the function -accept- return a new fd that will be used for communication, leaving the original free to accept other incoming connections