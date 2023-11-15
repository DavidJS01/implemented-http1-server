I will implement an HTTP 1.1 server in this project. 


ON REQUEST, get stream of bytes over network -> decode bytes as text (utf-8) -> tokenize data -> parse tokenized data

Requirements
- Parsing message formats

Out of scope 
- Backwards compatibility with HTTP/1.0 recipients
- Intermediary recipients checking version (no Intermediary recipients for this)
- Supporting request targets outside of origin-form

https://datatracker.ietf.org/doc/html/rfc9112#name-introduction

# Message Format
```
  HTTP-message   = start-line CRLF
                   *( field-line CRLF )
                   CRLF
                   [ message-body ]

```

CRLF = \r\n 

field-line = one or more header "field lines", referred to as headers

the message body is optional

The difference in message sending between client and server is simply that
requests's start-line is a request-line.

A response's start-line is a status-line

# Message Parsing
1. Read the start line into a structure
2. Read each header field line into a hash table by field name until the empty line
3. Use parsed data to determine if a message body is expected

If message body is expected, read it as a stream until an amount of octets equal to the message body length is read or the connection is closed 

## Requirements
- Recipient MUST parse HTTP message as sequence of octets in an encoding that is a superset of US-ASCII
- Sender MUST NOT generate a CR only character (without a following LF) in any protocol elements OTHER THAN THE MESSAGE.
- If sender sends the 2nd bullet point, recipient MUST consider that element to be invalid
- Recipient SHOULD ignore at least one empty CRLF recieved prior to the request-line
- Recipient that recieves whitespace between start line and first header field MUST reject the message as invalid 
- Recipient of a sequence of octets that does not match the HTTP-message grammar SHOULD respond with a 400

# Request Line 
Request line begins with a method token, followed by a single space, the request-target, and another single space, and then ends with protocol version.

request-line   = method SP request-target SP HTTP-version

# Method 
Method token indicates request method on resource. Case-sensitive.

Should be one of:
- GET 
- POST 
- PUT
etc.

# Request Target 
request-target identifies the target resource upon which to apply the request 

client dervices a request-target from its desired target URI 

four distinct formats for request-target: 
1. origin-form / absolute-form / authority-form / asterisk-form

client must send host header field in all requests; if no host header field respond 400

# Origin Form 
origin-form = absolute-path ["?" query]


For example, a client wishing to retrieve a representation of the resource identified as

  http://www.example.org/where?q=now

directly from the origin server would open (or reuse) a TCP connection to port 80 of the host "www.example.org" and send the lines:

GET /where?q=now HTTP/1.1
Host: www.example.org

# Status Line 

The first line of a response message is the status-line, consisting of the protocol version, a space (SP), the status code, and another space and ending with an OPTIONAL textual phrase describing the status code.

  status-line = HTTP-version SP status-code SP [ reason-phrase ]

Server MUST send space that seps the status code from the reason phrase even when reason phrase is empty
