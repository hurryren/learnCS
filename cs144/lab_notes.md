# lab0





This feature is known as a *stream socket*. To your program and to the Web server, the socket looks like an ordinary file descriptor (similar to a file on disk, or to the stdin or stdout I/O streams). When two stream sockets are connected, any bytes written to one socket will eventually come out in the same order from the other socket on the other computer.

In readlity, however, the Internet doesn’t provide a service of reliable byte-streams. Instead, the only thing the Internet really does is to give its “best effort” to deliver short pieces of data, called *Internet datagrams*, to their destination. Each datagram contains some metadata(headers) that specifies things like the source and destination addresses--what computer it came from, and what computer it’s headed towards-as well as some *payload* data (up to about 1,500 bytes) to be delivered to the destination computer.

Although the network tries to deliver every datagram, int practice datagrams can be:

1. lost
2. delivered out of order
3. delivered with the contains altered, or even
4. duplicated and delivered more thatn once.





# lab1



# lab2

In addition to writing to the incoming stream, the **TCPReceiver** is responsible for telling the sender two things:

1. the index of the “first unassembled” byte, whihc is called the “acknowledgement number” or “**ackno**.” This is the first byte that the receiver nuueds from the sender.
2. The distance between the “first unassembled” index and the “first unaccesptable” index. This is called the “**window size**”.

Together, the **ackno** and **window size** describle the receiver’s **window**: a **range of indexes** that the TCP sender is allowed to send. Using the window, the receiver can control the flow of incoming data, making the sender limit how much it sends until the receiver is ready for more. We sometimes refer to the ackno as the “left edge” of the window (smallest index the **TCPReceiver** is interested in), and the ackno + window size as the “right edge” just beyond the largest index the **TCPReceiver** is interested in.

These signals are crucial to TCP’s ability to provide the service of a flow-controlled, reliable byte stream over an unreliable datagram network. In TCP, **acknowledgment** means, “What is the index of the *next* byte that the receiver needs so it can reassemble more of the ByteStream?” This tells the sender what bytes it needs to send or resend. Flow control means, “What range of indices is the receiver interested and willing to receive?”(usually as a function of its remaining capacity). This tells the sender how much it’s allowed to send.



In the TCP headers, however, space is precious, and each byte’s index in the stream is represente not with a 64-bit index but with a 32-bit sequence number or seqno. this adds three complexities:

1. Streams in TCP can be arbitrarity long--there’s no limit to the length of a ByteStream that can be sent over TCP. But $2^{32}$ bytes is only 4 GiB, which is not so bit. Once a 32-bit sequ number counts up to $2^{32}-1$, the next byte in the stream will have the sequence number zero.
1. TCP sequence numbers start at a random value: To improve security and avoid getting confused by old segments belonging to earlier connections between the same endpoints, TCP tries to make sure sequence numbers can not be guessed and are unlikely to repeat. So the sequence numbers for a stream do not start at zero. The first sequence number in the stream is a random 32-bit number called the Initial Sequence NUmber(ISN). This is the sequence number that represents the SYN(beginning of stream). The rest of the sequence numbers behave normally after that: The first byte of data will have the sequence number of the ISN+1(mod $2^{32}$), the second byte will have the ISN+2(mod $2^{32}$), etc.
1. The logical beginning and ending each occupy one sequence number: In addition to ensuring the receipt of all bytes of data, TCP makes sure that the beginning and ending of the stream are received reliably. Thus, in TCP the SYN(beginning-of-stream) and FIN(end-of-stream) control flags are assigned sequence numbers. Each of these occupies one sequence number. (The sequence number occupied by the SYN flag is the ISN.) Each byte of the data in the stream also occupies one sequence number. Keep in mind that SYN and FIN aren’t part of the stream itself and aren’t “bytes” --they represent the beginning and ending of the byte stream itself.



| Sequence number  | Absolute sequence numbers | stream indices       |
| ---------------- | ------------------------- | -------------------- |
| Start at the ISN | start at 0                | start at 0           |
| include SYN/FIN  | include SYN/FIN           | omit SYN/FIN         |
| 32bits wrapping  | 64bits, non-wrapping      | 64bits, non-wrapping |
| seqno            | absolute seqno            | stream index         |





























