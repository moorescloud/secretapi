#ifdef IMX
#include "gpio-mmap.h"
	// GPIO 0  = SSP2_MISO
	// GPIO 16 = Arduino RESET
	// GPIO 19 = SSP2_SS = Arduino SS
	// GPIO 20 = SSP2_MOSI = LED DATA
	// GPIO 23 = Arduino "ACK/BUSY"
	// GPIO 24 = SSP2_SCK = LED CLOCK
	// GPIO 65 = LED
#else
#include <linux/spi/spidev.h>
#endif

#ifndef IMX
static const char *device = "/dev/spidev0.0";
static uint32_t speed = 500000;
static uint8_t mode;
static uint16_t delay;
static int spi_fd;
#endif

static uint8_t bits = 8;

static void pabort(const char *s)
{
	perror(s);
	abort();
}

void spi_open()
{
	//printf("spi_openi\n");
#ifdef IMX
	gpio_map();
	GPIO_WRITE_PIN(16,1);
	GPIO_WRITE_PIN(19,1);
	GPIO_WRITE_PIN(20,0);
	GPIO_WRITE_PIN(24,0);
	GPIO_WRITE_PIN(65,0);
	gpio_input(0,0);   //bank 0 bit 0  = GPIO0  = SSP2_MISO
	gpio_output(0,16); //bank 0 bit 16 = GPIO16 = Arduino RESET
	gpio_output(0,19); //bank 0 bit 19 = GPIO19 = SSP2_SS
	gpio_output(0,20); //bank 0 bit 20 = GPIO20 = SSP2_MOSI
	gpio_input(0,23);  //bank 0 bit 23 = GPIO23 = Arduino ACK/BUSY
	gpio_output(0,24); //bank 0 bit 24 = GPIO24 = SSP2_SCLK
	gpio_output(2,1);  //bank 2 bit 1  = GPIO65 the LED on board
#else
	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
* bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
#endif
}

void spi_close()
{
#ifndef IMX
	close(spi_fd);
#endif
}

static void spi_send(uint8_t txbuf[], int buffsize)
{
	//printf("spi_send\n");

#ifdef IMX
	uint8_t i, j, v, b;

	if (GPIO_READ_PIN(23)==0) return;  // Arduino still busy

	GPIO_WRITE_PIN(65,1); // LED on
	GPIO_WRITE_PIN(19,0); // SS active

	j = 0;
	while (GPIO_READ_PIN(23)==1 && j++<255) ; // wait for Arduino ack
	if (GPIO_READ_PIN(23)==1) {
		GPIO_WRITE_PIN(19,1); // SS inactive
		return;  // Arduino not responding busy
	}

	for (i=0; i<buffsize; i++) {
		v = txbuf[i];
		b = 0x80;
		for (j=0; j<bits; j++) {
			GPIO_WRITE_PIN(20,v&b); // set data
			b >>= 1;
			GPIO_WRITE_PIN(24,1); // clock active
			GPIO_WRITE_PIN(24,0); // clock inactive
		}
		// inter byte delay for Arduino SPI to keep up...
		// approx 125ns per loop, so 40 = ~5us
		for (j=0; j<140; j++)
		{
			GPIO_WRITE_PIN(24,0); // clock inactive
		}
	}
	GPIO_WRITE_PIN(19,1); // SS inactive
	GPIO_WRITE_PIN(65,0); // LED off
#else
	int ret;
	int j  = 0;
	int offset;
	uint8_t rx[385] = { 0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)txbuf,
		.rx_buf = (unsigned long)rx,
		.len = buffsize,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	
	//printf("buffsize %d\n", buffsize);
	ret = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
#endif
}

