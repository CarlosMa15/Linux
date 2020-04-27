Created on Jan 26, 2019
Author: Carlos Humberto Martinez Guadarrama

Phase 1:
This python class was resigned to act like a proxy. This proxy only sends 1 message it receives from
the client send it to the remote server then sends the respond it receives from the remote server to
the client. This proxy only can handle 1 message at the moment.

Phase 2: The proxy was designed to handle multiply clients. The
proxy now does a check on the headers and sends them
over in the request. The proxy Also takes the resonse
from the remote server and changes the response to any
use of the word simple it gets changed to the word silly