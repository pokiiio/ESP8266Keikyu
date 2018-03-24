#ifndef ESP8266KEIKYU_H
#define ESP8266KEIKYU_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ESP8266Keikyu
{
public:
  int getStatus();                    // 下で定義している定数を返します
  String getUnkoInfo();               // 京急のページから運行情報の文字列を取得します
  int getUnkoStatus(String unkoInfo); // 運行情報の文字列からステータスを判断します
private:
  String getUnkoInfoWithOffset(int offset); // Rangeを指定してHTTP-GETします
};

extern ESP8266Keikyu Keikyu;

#define KEIKYU_STATUS_UNKNOWN 0           // 京急の運行情報が取得できません
#define KEIKYU_STATUS_NORMAL 1            // 京急は平常通り運転してます
#define KEIKYU_STATUS_DELAYED 2           // 遅れています
#define KEIKYU_STATUS_TERRIBLE 3          // ダイヤが乱れています
#define KEIKYU_STATUS_STOPPED 4           // 運転を見合わせています
#define KEIKYU_STATUS_DELAYED_DUE_TO_JR 5 // JRからの振替輸送受託のため遅れています

#endif
