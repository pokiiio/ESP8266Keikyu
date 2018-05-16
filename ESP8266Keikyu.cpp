#include <ESP8266HTTPClient.h>
#include "ESP8266Keikyu.h"

int unkoInfoOffset = 0;

// unko、unkoと叫んでいるがふざけているわけではない
// 運行情報のページで、運行情報が書かれているdivのクラス名がunko-panelだからである

int ESP8266Keikyu::getStatus()
{
  return getUnkoStatus(getUnkoInfo());
}

String ESP8266Keikyu::getUnkoInfo()
{
  // 運行情報はこのコメントに囲まれている
  // コメントが変わると死ぬ
  String unkoPanelCommentStart = "<div class=unko-panel>";
  String unkoPanelCommentEnd = "</div>";

  // 既存のオフセットでHTTP-GETする
  String result = getUnkoInfoWithOffset(unkoInfoOffset);

  // 既存のオフセットでHTTP-GETした時に、特定のコメントが含まれていなかったら、
  // オフセットを0に戻して、ローラー作戦を開始する
  if (result.indexOf(unkoPanelCommentStart) < 0)
  {
    unkoInfoOffset = -500;

    do
    {
      delay(1000);
      unkoInfoOffset += 500;
      result = getUnkoInfoWithOffset(unkoInfoOffset);
    } while (result.indexOf(unkoPanelCommentStart) < 0);
  }

  result = result.substring(result.indexOf(unkoPanelCommentStart) + unkoPanelCommentStart.length());
  result = result.substring(0, result.indexOf(unkoPanelCommentEnd));

  return result;
}

String ESP8266Keikyu::getUnkoInfoWithOffset(int offset)
{
  HTTPClient http;

  // ページをすべてGETしようとすると落ちるので、ページの先頭からではなく途中からGETする
  int rangeSize = 1000;

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
