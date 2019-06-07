### Connected vs Unconnected UDP Sockets
- A client can call connect with a UDP socket or not
- If connect is called read and write will work
- Without connect the client needs to send with a system call specifying a remote endpoint
- Without connect it might be useful to receive data with a system call which tells the remote endpoint
- Connect with TCP involves a special message exchange sequence
- Connect with UDP sends no messages
- You can connect to non-existent servers

[参考链接](http://orca.st.usm.edu/~seyfarth/network_pgm/net-6-7-11.html)