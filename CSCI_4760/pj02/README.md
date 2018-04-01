# ReadMe

David Luo

811357331

Basic DNS client.

## To Run

### Activate virtualenv

From this directory:

```
source bin/activate
```

### Run Parser

```
usage: dns_client.py [-h] [-t TYPE] [--tcp] [--port PORT] [--timeout TIMEOUT]
                     [--retries RETRIES]
                     DNSIP HOST

positional arguments:
  DNSIP              Address of DNS server.
  HOST               DNS QName

optional arguments:
  -h, --help         show this help message and exit
  -t TYPE            DNS QType
  --tcp              If set, use TCP instead of UDP.
  --port PORT        Port on DNS server.
  --timeout TIMEOUT  Timeout on connection operations.
  --retries RETRIES  Number of times to retry querying DNS server.
```

### Sample output

```
$ python dns_client.py 45.79.221.152 google.com
```
```
; <<>> dns_client.py David Luo 811357331 <<>> 45.79.221.152 google.com
; (1 server found)
;; Got answer:
;; ->>HEADER<<- opcode: 0, status: NOERROR, id:27449
;; flags: QR RD; QUERY: 1, ANSWER: 7, AUTHORITY: 7, ADDITIONAL: 0

;; QUESTION SECTION:
;google.com.                            IN      A

;; ANSWER SECTION:
foo.google.com.             3600        IN      A           192.168.1.0
bar.google.com.             3600        IN      A           192.168.1.1
wks.google.com.             60          1       11          127.0.0.1 6 2 27
minfo.google.com.           42          IN      MINFO       rbx.google.com. ebx.google.com.
ptr.google.com.             12          IN      PTR         ptrd.google.com.
ns.google.com.              12          IN      NS          nsd.google.com.
ns.google.com.              12          IN      TXT         "never gonna give you up"

;; AUTHORITY SECTION:
biz.google.com.             3600        IN      A           192.168.1.0
baz.google.com.             3600        IN      A           192.168.1.1
wks.google.com.             60          1       11          127.0.0.1 6 2 27
minfo.google.com.           42          IN      MINFO       rbx.google.com. ebx.google.com.
ptr.google.com.             12          IN      PTR         ptrd.google.com.
ns.google.com.              12          IN      NS          nsd.google.com.
ns.google.com.              12          IN      TXT         "never gonna let you down"

;; Query time: 36 msec
;; SERVER: 45.79.221.152#53(45.79.221.152)
;; WHEN: Fri Mar 23 21:21:41 Eastern Daylight Time 2018
;; MSG SIZE  rcvd: 385
```
