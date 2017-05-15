#include <aJSON.h>
aJsonStream serial_stream(&Serial);

String songS = "";
String musicS = "";
String projtctid = "";

String ID = "{\"ID\":\"590bc5e87bb3e300016e2da2\"}";

//{"ID":"project"}
void IDupload()
{
  Serial.print(ID.substring(0, 17));
  delay(30);
  Serial.println(ID.substring(17, 33));
  delay(30);
}

void Ctrldata(String _st, int _data)
{
  String send_data;
  int number;
  send_data = "{\"";
  send_data += _st;
  send_data += "\":\"";
  send_data += _data;
  send_data += "\"\}";
  number = send_data.length() / 17;

  if (number == 0)
  {
    Serial.println(send_data);
    delay(30);
  }
  else
  {
    while (number >= 0)
    {
      Serial.print(send_data.substring(0, 17));
      send_data = send_data.substring(17, send_data.length());
      delay(30);
      number--;
    }
    Serial.print("\n");
  }
}

//{"songIndex":"3"}
//{"music":"1"}

void ComExecution(aJsonObject *msg)
{
  aJsonObject *IDval = aJson.getObjectItem(msg, "ID");
  if (IDval) {
    projtctid = IDval->valuestring;
    if (projtctid == "project")
    {
      IDupload();
      Ctrldata("songIndex", playNum);
    }
  }
  else {
#if DEBUG
    Serial.println("no ID data");
#endif
  }

  aJsonObject *songval = aJson.getObjectItem(msg, "songIndex");
  if (songval) {
    songS = songval->valuestring;
    playNum = songS.toInt();
    playIndex = 0;
    write_eeprom();
  }
  else {
#if DEBUG
    Serial.println("no songIndex data");
#endif
  }
  aJsonObject *musicval = aJson.getObjectItem(msg, "music");
  if (musicval) {
    musicS = musicval->valuestring;
    playtone = musicS.toInt();
    if (playtone == -1)
      playtone = 0;
  }
  else {
#if DEBUG
    Serial.println("no music data");
#endif
  }

#if DEBUG
  Serial.println("--------------");
  Serial.print("songIndex DATA:");
  Serial.println(playNum);
  Serial.print("music DATA:");
  Serial.println(playtone);
  Serial.println("--------------");
#endif
}

void musicboxjson_data()
{
  if (serial_stream.available()) {
    serial_stream.skip();
  }
  if (serial_stream.available()) {
    aJsonObject *msg = aJson.parse(&serial_stream);
    ComExecution(msg);
    aJson.deleteItem(msg);
  }
}
