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

