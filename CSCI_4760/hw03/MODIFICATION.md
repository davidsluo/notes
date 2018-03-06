# RF4760 Modifications

The protocol could be modified so that there is an optional `Host Port` field,
which would specify a port on the host address that recieves responses from
the destination. The protocol header would be modified as follows:


```
 0             15 16            31
+----------------+----------------+
|            Host Port            |
+----------------+----------------+
|        Destination Port         |
+----------------+----------------+
|     Length     | Data Octets... |
+----------------+----------------+
```

The `Host Port` field, similar to the `Destination Port` field, would be a
32-bit unsigned integer. If the Host Port is not needed, this field should
be all zeros.