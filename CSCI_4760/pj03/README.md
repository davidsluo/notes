# Read Me

David Luo

811357331

Multi-threaded file transfer client/server.

## To Run

### Activate virtualenv

From this directory:

```
$ source bin/activate
```

### Run Server

```
$ python3 ftserver.py -h
```
```
usage: ftserver.py [-h] [--static-id STATIC_ID] port

positional arguments:
  port                  The port to host the server on.

optional arguments:
  -h, --help            show this help message and exit
  --static-id STATIC_ID
                        Force the server to provide any client that connects
                        with the specified ID. Generally used only in testing.
```

### Run Client

```
$ python3 ftclient.py -h
```
```
usage: ftclient.py [-h] --server HOST:PORT
                   (--receive [COUNT] | --send ID filename ID filename)
                   [-c CNUM] [-s SIZE] [-p PORT] [--log-level LOG_LEVEL]

optional arguments:
  -h, --help            show this help message and exit
  --server HOST:PORT    specifies the host and port of the tracker server.
  --receive [COUNT]     indicates that the client is in "receive" mode. May
                        also specify how many files to receive. -1 for
                        unlimited (default -1)
  --send ID filename ID filename
                        indicates that the client is in "send" mode.
  -c CNUM, --cons CNUM  specifies the number of parallel, concurrent
                        connections to use when sending (default 1).
  -s SIZE, --size SIZE  specifies the size of the buffer size when receiving
                        (default 4096).
  -p PORT, --port PORT  specifies the port that the client will use for
                        receiving (default to 47685).
  --log-level LOG_LEVEL
                        specifies the level of logging (default INFO).
```

#### Receive a file.

```
$ python3 ftclient.py --receive <server host:server port>
```

This will specify an ID that must be provided to the sender(s).

#### Receive a file with chunk size 8192.

```
$ python3 ftclient.py --receive <server host:server port> -s 8192
```

#### Send a file.

```
$ python3 ftclient.py --send <ID> <filename> <server host:server port>
```

Sends `filename` to client receiving on `ID`.

#### Send a file over 10 connections.

```
$ python3 ftclient.py --send <ID> <filename> <server host:server port> -c 10
```

### Example

On vcf3:

```
$ python3 ftserver.py 4760
```

On anywhere that can access port 4760 on vcf3:

```
$ python3 ftclient.py --receive vcf3 4760
```
This gives us some ID between 0 and 65535. Let's use 48.

On anywhere that is on the same network as the above client.

```
$ python3 ftclient.py --send 48 cat.jpg vcf3:4760
```

`cat.jpg` will now be transfered to the receiving client.

## Running Tests

1. First, have the server running somewhere accessible by two other servers.
   This can itself be run on either of the clients or a different machine completely.
2. Specify in both `test_reciever.py` and `test_sender.py` the address of the negotiating server.
3. Run `python3 test_receiver.py`.
4. Run `python3 test_sender.py`

This will start to run the benchmarks with the provided settings. Edit the
`test_receiver.py` and `test_sender.py` to change test parameters.

