#ifndef _TEMPERATURETEST_H_
#define _TEMPERATURETEST_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>


int spi_init(char filename[40]); 
char *spi_read_lm74(int file);
void temp_read();  


#endif
