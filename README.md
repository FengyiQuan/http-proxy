# Http Proxy

## Danger Log
1. To prevent partial data transmission over a network, we implement `send_data` function to wrap the socket send operation, ensuring that the entire message is sent before returning to the caller.