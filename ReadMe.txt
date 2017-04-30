Meiryo UIも大っきらい!! source file package
By Tatsuhiko Syoji(Tatsu) 2005,2012-2017

[必要となるプログラム]
・Visual Studio 2008/2015以降(VC++)

[ファイル]
ReadMe.txt:本ファイルです。
LICENSE.txt : MITライセンスの条文です。

・programフォルダ配下のファイル
noMeiryoUI_2008.sln : Visual Studio 2008用ソリューションファイルです。
noMeiryoUI_2008.vcproj : Visual Studio 2008用プロジェクトファイルです。
noMeiryoUI.sln : Visual Studio 2015用ソリューションファイルです。
noMeiryoUI.vcxproj : Visual Studio 2015用プロジェクトファイルです。
noMeiryoUI.vcxproj.filters : Visual Studio 2015用プロジェクトファイルです。
TWRフォルダ配下のファイル : GUI構築に使用したファイルです。
その他のファイル : Meiryo UIも大っきらい!!自身のソースファイルです。

・helpフォルダ配下のファイル
ja-jpフォルダ配下のファイル : 日本語版ヘルプファイルのソースファイルです。
en-usフォルダ配下のファイル : 簡単な英語版ヘルプファイルのソースファイルです。
zh-cnフォルダ配下のファイル : 立花 月下様訳による簡体字中国語版ヘルプファイルのソースファイルです。

・langフォルダ配下のファイル
ChineseSimplified.lng : 立花 月下様訳による簡体字中国語の言語ファイルです。
English.lng : 英語の言語ファイルです。
default.lng : 言語ファイルのサンプルを兼ねた簡体字中国語・英語以外のときに使用される言語ファイルです。

[使い方]
・プログラムのビルド
1.まず、本アーカイブをディレクトリ付きで展開します。
2.programフォルダのnoMeiryoUI_2008.sln(Visual Studio 2008)か
　noMeiryoUI.sln(Visual Studio 2015)を開きます。
3.ビルドを行うと実行ファイルがデバッグ版はDebugフォルダに、リリース版は
　Releaseフォルダに生成されます。

・ヘルプの訳
1.編集元となるヘルプファイルをhelpフォルダの下のフォルダごとコピーしてください。
2.contentsフォルダ内のHTMLファイルを訳してください。
3..hhpファイルをHTML Helo workshopで開いて編集し、ヘルプファイルを作成してください。

・言語ファイルの訳
1.helpフォルダ内の言語ファイルをコピーしてください。
2.[RESOURCE]と書かれた行のキー=値となっている部分の値の部分を変更します。
　FONT_FACE=System
　とある行が表示に使用するフォント名になるので=の後ろにフォント名を書いてください。
　FONT_CHARSET=1
　とある行は表示に使用するフォントの文字セットになりますので、文字セットに対応する数値を指定してください。
　上記以外はメッセージとなりますので、画面の表示と照らし合わせて訳してください。
　例)
　DLG_ALL_FONT=All fonts

　文字セットに対応する数値は以下の通りとなります。
　ANSI文字セット:0
　デフォルト文字セット(通常はこれを指定します。)：1
　シンボル：2
　日本語(SHift-JIS)：128
　ハングル文字：129
　簡体字中国語(GB2312)：134
　繁体字中国語(BIG5)：136
　OEM文字セット：255
　組合型符号化文字集合ハングル文字：130
　ヘブライ語：177
　アラビア語：178
　ギリシア語：161
　トルコ語：162
　ベトナム語：163
　タイ語:222
　東欧文字セット：238
　ロシア語文字セット：204
　MAC文字セット：77
　バルト文字セット：186

3.[PRESET]と書かれた行以下の内容はプリセットメニューのフォントの情報です。
　各OSの初期状態のフォント名とフォントサイズを記述してください。
　CAPTIONで始まる行はタイトルバーのフォント情報です。
　ICONで始まる行はアイコンのフォント情報です。
　SMALLCAPTIONで始まる行はパレットのフォント情報です。
　STATUSで始まる行はステータスバー・ヒントのフォント情報です。
　MESSAGEで始まる行はメッセージボックスのフォント情報です。
　MENUで始まる行はメニューのフォント情報です。
　
　FACEが入る行にはフォント名を記述してください。
　SIZEが入る行にはフォントサイズを記述してください。
　CHARSETが入る行には文字セットを記述してください。
　後ろが_8で終わる行にはWindows 8/8.1の、_10で終わる行はWindows 10の
　フォントの情報を記述してください。
　例)タイトルバーがSegoe UIの11ポイント、文字セットデフォルト、Windows 8.1の場合
　CAPTION_FACE_8=Segoe UI
　CAPTION_SIZE_8=11
　CAPTION_CHARSET_8=1

ヘルプ・言語ファイルを訳した場合、プログラムに組み込む必要がありますので、
下記の作者のほうにご連絡ください。

もしくは、utilディレクトリのlocaletest.exeをコマンドプロンプトから
実行した結果を参考にnoMeiryoUI.cppのinitializeLocale関数の
// If you want to add language support, add language detection, language file,
// and help file name.
とある行から
// Language support routine ends here.
とある行までの間の言語判定ルーチンにlocaletest.exeを実行して表示された
言語名にマッチする場合の処理を追加してください。

[ライセンス]
MITライセンス(http://opensource.org/licenses/mit-license.php)に従います。

[作者]
庄子 達彦(Tatsu)
E-mail Address rxk16664@nifty.ne.jp
My Web page http://tatsu.life.coocan.jp/index.html
------------------------------------------------------------------------
