#ifndef __EEPROM_H__
#define __EEPROM_H__
unsigned char EepromReadByte(unsigned char addr);
void EepromWriteByte(unsigned char addr, unsigned char dat);
#endif