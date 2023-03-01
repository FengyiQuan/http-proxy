# Http Proxy

An http proxy – a server whose job it is to forward requests to the origin server on behalf of the client. Your proxy will cache responses, and, when appropriate, respond with the cached copy of a resource rather than re-fetching it.

## Danger Log

- We set the basic strucutre and use OOD to have `Parser`, `Logger`, `Request`, `Response` to have a better maintainace cycle.
- To prevent partial data transmission over a network, we implement `send_data` function to wrap the socket send operation, ensuring that the entire message is sent before returning to the caller.
- Since the request line could be a relative path or absolute path, a proxy needs to handle the response appropriately, to map the URI to the correct format. This is handled on `cache.cpp`.
- To avoid malformed data from both client and server, parser would throw exception to notify proxy to send 400/502 to client/server.
- We faced multiple empty package from client and server issue and we solved it by checking received message at the first place.
- To implement cache, request can be either relative or absolute path. This makes implementing cache hard. Thus, we add a new helper function in request to get absolute path to better support cache store as key value.
- To prevent send partial data issue, we wrap the basic socket send to `send_data` function to continue check if the entire data is sent successfully.
- The exception guarantees we make is basic: Objects remain in valid states, no dangling pointers, invariants remain intact, no memory is leaked. This is tested in valgrind.
- To better address we followed good C++ style. We are using C++11 `std::thread` and `std::lock_guard` to prevent never release lock issue.
- Socket is closed after finished to prevent uncertain behavior.
- Lock cache object to prevent concurrent access cache map.

## Robust to external failures

- If it contacts the destination webserver and receives a corrupted response, it MUST reply with a 502 error code.
- If the proxy receives a malformed request, it MUST reply with a 400 error code.

## Functionality

- support GET
- support POST
- support CONNECT
- cache appropriate responses
- follow rules of expiration time
- follow rules of re-validation
- one thread per request
- concurrent request
- logger
- docker
