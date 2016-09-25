Meiryo UIも大っきらい!! source file package
By Tatsuhiko Syoji(Tatsu) 2005,2012-2016

[必要となるプログラム]
・Visual Studio 2008/2015(VC++)

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

・langフォルダ配下のファイル
ChineseSimplified.lng : 立花 月下様訳による簡体字中国語の言語ファイルです。
English.lng : 言語ファイルのサンプルを兼ねた英語の言語ファイルです。

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
　とある行がフォント名になるので=の後ろにフォント名を書いてください。
　上記以外はメッセージとなりますので、画面の表示と照らし合わせて訳してください。
　例)
　DLG_ALL_FONT=All fonts

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
　例)タイトルバーがSegoe UIの11ポイントの場合
　CAPTION_FACE_8=Segoe UI
　CAPTION_SIZE_8=11


ヘルプ・言語ファイルを訳した場合、プログラムに組み込む必要がありますので、
下記の作者のほうにご連絡ください。

[ライセンス]
MITライセンス(http://opensource.org/licenses/mit-license.php)に従います。

[作者]
庄子 達彦(Tatsu)
E-mail Address rxk16664@nifty.ne.jp
My Web page http://tatsu.life.coocan.jp/index.html
------------------------------------------------------------------------
