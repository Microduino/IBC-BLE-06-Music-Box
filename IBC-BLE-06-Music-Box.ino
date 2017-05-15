#include <Microduino_Key.h>
#include "colorLed.h"
#include "music.h"
#include "userDef.h"
#include "eeprom.h"
#include "musicboxJson.h"

Key keyLine(PIN_LINEA, INPUT_PULLUP);

uint32_t cardTimer = 0;
uint8_t cardNum = 0;

void getCard()
{
  if (keyLine.read(0, DOCK_MAX) == SHORT_PRESS)
  {
    setAllColor(COLOR_BLUE);
    cardNum++;
    cardTimer = millis();
  }
}

void tonemusic(uint8_t fre)
{
  if (fre == 0)
    noTone(PIN_BUZZER);
  else
    tone(PIN_BUZZER, toneList0[fre]);
  ledBlink(100, fre, 0);
}

void setup() {
  Serial.begin(57600);
  strip.begin();
  strip.setBrightness(BRIGHT_MAX);
  read_eeprom();
  if (playNum < 0 || playNum > 11)
  {
    playNum = 2;
  }
#if DEBUG
  Serial.println("**************START************");
#endif
}

void loop() {
  //  Serial.print("LINE Val:");
  //  Serial.println(analogRead(PIN_LINEA));      //灰度传感器检测到的亮度。
  getCard();
  musicboxjson_data();
  if (millis() - cardTimer > 1000 && cardTimer > 0) //1秒钟内没有检测到新的条纹，即认为刷卡结束。
  {
    noTone(PIN_BUZZER);
    setAllLed(COLOR_NONE);
    playNum = constrain(cardNum - 2, 0, SONG_SUM - 1); //根据扫描到的黑色条纹数播放相应的音乐。
    cardNum = 0;
    playIndex = 0;
    cardTimer = 0;
    allLedBlinkNum(playNum + 1, COLOR_WARM, 500);
  }
  if (!playSound(playNum))//播放音乐。
  {
    //ledBreath(COLOR_GREEN, 15);
    tonemusic(playtone);
  }
  delay(15);
}
