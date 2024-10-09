□設備アラーム状況一覧用ツール（SharedAlarm.exe）の使用方法

モジュール名：SharedAlarm.exe

内容；リモート端末アプリ（StationControl.exe）内の「設備アラーム状況一覧」画面に
	表示するために、FA-Panel上で作成されたCSVファイルの内容を共有メモリに書き込むツール。

引数：/DEBUG /FILE:アラームファイル名（フルパス名）
	/DEBUG：GUIでアラームファイルの送受信を行える
	/FILE：完全パスで各局毎のCSVファイル名を指定します

使用方法：
 (1) デバッグモードでリモート端末に設備アラーム状況を表示する場合
　　①リモート端末アプリを１局モード（臼田64m局）で起動する
　　②SharedAlarm.exe /DEBUGで実行
　　③「起動モード」で「Client(FA-Panel)」を選択して「OK」ボタンを押下
　　④「選択...」ボタンを押下して、アラームファイルを選択する
　　⑤「Send」ボタンを押下して共有メモリに登録する
（2) 臼田64m局の設備アラーム状況を表示する場合
　　①リモート端末アプリを１局モード（臼田64m局）で起動する
　　②SharedAlarm.exe /FILE:C:\FA-Panel\PROJECT\UDSC64\U64_20170830010203_500.csvで実行


□設備アラーム状況一覧画面用の設備状況一覧ファイルフォーマット

・ファイル名
　XXX_YYYYMMDDhhmmss_msc.csv
　　XXX：局識別（U64、U34、U20、U54）
　　YYYYMMDDhhmmss：西暦年月日時分秒
　　msc：ミリ秒

・ファイルの内容
１行目：
　ファイルの更新日時。
　　例）
　　　「2017-08-30 01:02:03」
　　更新日時に発生している設備アラームの一覧が2行目以降にリストされます。

２行目以降：
　１行で１設備アラーム項目の情報を表し、発生した設備アラーム項目数分（行）が出力されます。
　１行は、局、装置名、ステータス、監視項目名、監視項目の値をカンマ「,」区切りで出力され、行末は改行（CR+LF）になります。
　文字コードはSJIS

臼田64m局,設備制御装置,LOCAL,ANT.CONT_MODE,LOCAL
臼田64m局,設備制御装置,FAULT,ANT.ANT_ALARM,ALARM
臼田64m局,設備制御装置,FAULT,ANT.CTRL_UNIT_ALARM,ALARM
臼田64m局,設備制御装置,FAULT,ANT.REAL_EL,89.0
臼田64m局,設備制御装置,POWER OFF,S-TXRF.TRANS_PWR,OFF
臼田64m局,設備制御装置,POWER OFF,S-TXRF.TX_PWR,OFF
臼田64m局,設備制御装置,WARNING,SS-TCR.DC_PWR_ALM,ON
臼田64m局,設備制御装置,WARNING,SS-TCR.DC_MONI_PWR_ALM,ON

・特記事項
－設備アラーム状況は、更新日時の時点で発生している設備アラーム状況を格納していますので、新しいファイルに更新する場合は、
前回表示している内容を全てクリアし、新しい設備アラーム状況ファイルの内容に置換える必要があります。

