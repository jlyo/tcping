Installation:
------------
make install


Usage:
-----
tcping hostname
(uses port 80) or
tcping -p port hostname

ping once:
tcping -p port -c 1 hostname

Return code is 0 for could connect (at least once) and 127 when no connection at all could be made.


examples:
tcping -p 8080 127.0.0.1

Please support my opensource development: http://www.vanheusden.com/wishlist.php
