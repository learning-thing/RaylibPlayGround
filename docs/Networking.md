Networking is Message and string based to keep it as simple as possible.

It is recommended to have a spereate Script for the server logic, in which the `Gost()` function.

Once a srever exists, a client can connect by address using the `Connect()` function.

Once a connection is established, the onMessage(message) function will be called whenever your script (wether server or client) receives a message. The argument message contains the content of that message.

