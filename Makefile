all: secretapi sender

secretapi: secretapi.c spi.h gpio-mmap.h
	gcc secretapi.c -o secretapi -DTARGET -DIMX
#	gcc secretapi.c -o secretapi
	sudo chown root:root secretapi
	sudo chmod 6711 secretapi

sender: sender.c
	gcc sender.c -o sender

clean:
	rm secretapi
	rm sender
