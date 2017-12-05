(This readme is available in Japanese.)

# ESP8266Keikyu
ESP8266で京急の運行情報を取得するライブラリです


## インストール

このライブラリを下記フォルダにダウンロード、もしくはCloneしてください。

```
 /Users/YOUR_USER_NAME/Documents/Arduino/libraries/
 (Macを使っているひとはここ。Winはよくわからない。)
```


## 使い方

このライブラリをインストールしたあとに、ESP8266向けのArdunoスケッチ上で、


```c
#include <ESP8266Keikyu.h>
```

こんな感じでincludeしてあげます。そして、Wi-Fiが使える状態（インターネット接続可能なアクセスポイントに接続済み）で、


```c
int keikyuStatus = Keikyu.getStatus();
```


この関数を呼ぶと、京急の運行状況がわかるというものです。


このステータスの詳細は以下のようになってます。



```c
#define KEIKYU_STATUS_UNKNOWN 0           // 京急の運行情報が取得できません
#define KEIKYU_STATUS_NORMAL 1            // 京急は平常通り運転してます
#define KEIKYU_STATUS_DELAYED 2           // 遅れています
#define KEIKYU_STATUS_TERRIBLE 3          // ダイヤが乱れています
#define KEIKYU_STATUS_STOPPED 4           // 運転を見合わせています
#define KEIKYU_STATUS_DELAYED_DUE_TO_JR 5 // JRからの振替輸送受託のため遅れています
```


こんな感じで、ちょっと細かめにステータスを分けています。この結果に応じて、LEDを光らせたり、色んなアクションを実行して、あなただけの京急ガジェットを作ってみて下さい！
