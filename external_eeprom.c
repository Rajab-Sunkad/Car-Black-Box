#include "main.h"
#include "i2c.h"
#include "external_eeprom.h"
void write_external_eeprom(unsigned char address, unsigned char data)
{
	i2c_start();
	i2c_write(0xA0);
	i2c_write(address);
	i2c_write(data);
	i2c_stop();
    for(unsigned long wait = 3000; wait--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
	unsigned char data;

	i2c_start();
	i2c_write(0xA0);
	i2c_write(address);
	i2c_rep_start();
	i2c_write(0xA1);
	data = i2c_read();
	i2c_stop();

	return data;
}