# ReadMe

David Luo

811357331

Parses DNS messages.

## To Run

### Activate virtualenv

From this directory:

`source bin/activate`

### Run Parser

`python3 dns_parse.py <filename>`

### Sample output

```
; <<>> dns_parse.py David Luo 811357331 <<>> test/google.com.A.RR.bin
; (1 server found)
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id:4697
;; flags: qr rd ra; QUERY: 1, ANSWER: 6, AUTHORITY: 0, ADDITIONAL: 1

;; QUESTION SECTION:
;google.com.                            IN      A

;; ANSWER SECTION:
google.com.                 299         IN      A           173.194.219.100
google.com.                 299         IN      A           173.194.219.102
google.com.                 299         IN      A           173.194.219.113
google.com.                 299         IN      A           173.194.219.138
google.com.                 299         IN      A           173.194.219.101
google.com.                 299         IN      A           173.194.219.139
```
