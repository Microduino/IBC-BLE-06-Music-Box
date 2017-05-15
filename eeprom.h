#include <EEPROM.h>

uint8_t playNum = 0;
int8_t playtone = 0;

//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int EEPROMsong;
};

void read_eeprom() {
  config_type config_readback;
  EEPROM_read(1, config_readback);
  playNum = config_readback.EEPROMsong;
}

void write_eeprom() {
  config_type config;                  // 定义结构变量config，并定义config的内容
  config.EEPROMsong = playNum;
  EEPROM_write(1, config);         // 变量config存储到EEPROM，地址0写入
}
