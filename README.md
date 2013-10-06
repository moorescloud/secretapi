SECRET API
==========

Holiday by MooresCloud provides a very fast UDP-based API to control its globes.  Port 9988 is opened and is
advertised through mDNS - the avahi service record for _secretapi._udp is included in this repo. 

The packet format is very simple, 160 (0xa0) bytes composed of: 

* 10 bytes of header (ignored at present, this will undoubtedly change)

* 150 bytes of data, the RGB values for each of the 50 globes.  

Packets not of exactly 160 bytes will be rejected.

Named in honor of the fine folks at Secret Labs in Hobart.

Mark Pesce

