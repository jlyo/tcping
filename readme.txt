Installation:
------------
make install


Usage:
-----
tcping -h hostname
(uses port 80) or
tcping -h hostname -p port

ping once:
tcping -h host -p port -c 1

Return code is 0 for could connect (at least once) and 127 when no connection at all could be made.


examples:
tcping -g http://localhost/
tcping -h 127.0.0.1 -p 8080

Please support my opensource development: http://www.vanheusden.com/wishlist.php
