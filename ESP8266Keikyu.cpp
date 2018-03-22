#include <ESP8266HTTPClient.h>
#include "ESP8266Keikyu.h"

// unko、unkoと叫んでいるがふざけているわけではない
// 運行情報のページで、運行情報が書かれているdivのクラス名がunko-panelだからである

int ESP8266Keikyu::getStatus()
{
  return getUnkoStatus(getUnkoInfo());
}

String ESP8266Keikyu::getUnkoInfo()
{
  HTTPClient http;

  // ページをすべてGETしようとすると落ちるので、ページの先頭からではなく途中からGETする
  // ページの構成が変わると死ぬ
  int offset = 20400;
  int rangeSize = 1000;

  // 運行情報はこのコメントに囲まれている
  // コメントが変わると死ぬ
  String unkoPanelComment = "<!-- ======================== 運行情報 =================================== -->";

  // 京急の運行ページをパースします
  http.begin("http://unkou.keikyu.co.jp/");

  // Rangeの設定をして、部分的にGETする
  http.addHeader("Range", "bytes=" + String(offset) + "-" + String(offset + rangeSize));

  int httpCode = http.GET();

  String result = "";

  if (httpCode > 0 && http.getSize() > rangeSize)
  {
    result = http.getString();
  }

  http.end();

  if (result.indexOf(unkoPanelComment) > 0)
  {
    result = result.substring(result.indexOf(unkoPanelComment) + unkoPanelComment.length());
    result = result.substring(0, result.indexOf(unkoPanelComment));
  }

  return result;
}

int ESP8266Keikyu::getUnkoStatus(String unkoInfo)
{
  // 含まれている文字列で運行状況を判断
  // 今後運用が変わったら死ぬ可能性がある

  if (unkoInfo.indexOf("受託") > 0)
  {
    return KEIKYU_STATUS_DELAYED_DUE_TO_JR;
  }

  if (unkoInfo.indexOf("見合わせ") > 0)
  {
    return KEIKYU_STATUS_STOPPED;
  }

  if (unkoInfo.indexOf("乱れ") > 0)
  {
    return KEIKYU_STATUS_TERRIBLE;
  }

  if (unkoInfo.indexOf("遅れ") > 0 || unkoInfo.indexOf("運休") > 0)
  {
    return KEIKYU_STATUS_DELAYED;
  }

  if (unkoInfo.indexOf("平常") > 0)
  {
    return KEIKYU_STATUS_NORMAL;
  }

  return KEIKYU_STATUS_UNKNOWN;
}

ESP8266Keikyu Keikyu;
