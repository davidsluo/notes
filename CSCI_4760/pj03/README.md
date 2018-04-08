# ReadMe

David Luo

811357331

Basic file transfer client/server.

## To Run

### Activate virtualenv

From this directory:

`source bin/activate`

### Run Server

```
$ python3 ftserver.py -h
```
```
usage: python3 ftserver.py [-h] host port

positional arguments:
  host  The host to run the server on.
  port  The port to run the server on.

optional arguments:
  -h, --help  show this help message and exit
```

### Run Client

```
usage: python3 ftclient.py [-h] (--receive | --send ID filename) host port

positional arguments:
  host                  The host of the server.
  port                  The port of the server.

optional arguments:
  -h, --help            show this help message and exit
  --receive             receive a file
  --send ID filename    send <filename> to receiving client with id <ID>
```

#### Receive a file.

```
$ python3 ftclient.py --receive host port
```

This will specify an ID that must be provided to the sender(s).

#### Send a file.

```
$ python3 ftclient.py --send <ID> <filename> host port
```

Sends `filename` to client receiving on `ID`.

### Example

On vcf3:

```
$ python3 ftserver.py 0.0.0.0 4760
```

On anywhere that can access port 4760 on vcf3:

```
$ python3 ftclient.py --receive vcf3 4760
```
This gives us some ID between 0 and 65535. Let's use 48.

On anywhere that is on the same network as the above client.

```
$ python3 ftclient.py --send 48 cat.jpg vcf3 4760
```

`cat.jpg` will now be transfered to the receiving client.
