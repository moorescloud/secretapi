all: secretapi sender

secretapi: secretapi.c spi.h gpio-mmap.h
	gcc secretapi.c -o secretapi -DTARGET -DIMX 
	sudo chown root:root secretapi
	sudo chmod 6711 secretapi
	sudo cp secretapi /home/holiday/bin/secretapi
	sudo chmod 6711 /home/holiday/bin/secretapi

sender: sender.c
	gcc sender.c -o sender

clean:
	rm secretapi
	rm sender
