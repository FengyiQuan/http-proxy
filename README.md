# Http Proxy

## Danger Log
1. To prevent partial data transmission over a network, we implement `send_data` function to wrap the socket send operation, ensuring that the entire message is sent before returning to the caller.
2. Since the request line could be a relative path or absolute path, a proxy needs to handle the response appropriately, to map the URI to the correct format. This is handled on `cache.cpp`
3. To avoid malformed data from both client and server, parser would throw exception to notify proxy to send 400/502 to client/server.