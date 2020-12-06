Meiryo UIも大っきらい!! source file package
By Tatsuhiko Syoji(Tatsu) 2005,2012-2020

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
en-usフォルダ配下のファイル : 英語版ヘルプファイルのソースファイルです。
zh-cnフォルダ配下のファイル : 簡体字中国語版ヘルプファイルのソースファイルです。
zh-twフォルダ配下のファイル : 繁体字中国語版ヘルプファイルのソースファイルです。
pt-brフォルダ配下のファイル : ポルトガル語(ブラジル)版ヘルプファイルのソースファイルです。
ko-krフォルダ配下のファイル : 韓国語版ヘルプファイルのソースファイルです。

・langフォルダ配下のファイル
English.lng : 英語の言語ファイルです。
Chinese (Simplified)_China.lng : 簡体字中国語の言語ファイルです。
Chinese (Traditional)_Taiwan.lng : 繁体字中国語の言語ファイルです。
Portuguese_Brazil.lng : ポルトガル語(ブラジル)の言語ファイルです。
Korean.lng : 韓国語の言語ファイルです。
default.lng : 言語ファイルのサンプルを兼ねた言語に合う言語ファイルがないときに使用される言語ファイルです。

[使い方]
・プログラムのビルド
1.まず、本アーカイブをディレクトリ付きで展開します。
2.programフォルダのnoMeiryoUI_2008.sln(Visual Studio 2008)か
　noMeiryoUI.sln(Visual Studio 2015以降)を開きます。
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
　ANSI文字セット(通常はこれを指定します。):0
　デフォルト文字セット：1
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

4.最後に、Meiryo UIも大っきらい!!が言語ファイルを読み込めるように
　言語ファイル・ヘルプファイルをリネームします。
　utilディレクトリのlocaletest.exeをコマンドプロンプトから
　実行すると 言語名_国名.コードページ の形式で現在のWindowsの表示等の
　言語が表示されます。
　言語名で表示する場合は'_'より前の文字列にそれぞれのファイルの拡張子を
　つけた名前に言語ファイルとヘルプファイルの名前を変更してください。
　言語名と国に合わせた表示を行う場合は'.'より前の文字列にそれぞれのファイルの
　拡張子をつけた名前に言語ファイルとヘルプファイルの名前を変更してください。
　ファイル名の変更が終わったらnoMeiryoUI.exeと同じフォルダに置くことで
　言語ファイルが読み込まれ、ヘルプファイルが表示されます。

　ただし、default.lng、default.hlpは表示する言語の言語ファイルがない場合に
　使用するので、残すようにしてください。

例)
・英語(国を問わない)の場合
English_United States.437
などと出るので、'_'の前のEnglishにそれぞれの拡張子を付けた名前に
ファイル名を変更します。
言語ファイル名はEnglish.lng
ヘルプファイル名はEnglish.chm
となります。

・簡体字中国語(中国)の場合
Chinese (Simplified)_China.936
と出るのでChinese (Simplified)_Chinaにそれぞれの拡張子を付けた名前に
ファイル名を変更します。
言語ファイル名はChinese (Simplified)_China.lng
ヘルプファイル名はChinese (Simplified)_China.chm
となります。

[ライセンス]
MITライセンス( http://opensource.org/licenses/mit-license.php )に従います。

[作者]
庄子 達彦(Tatsu)
E-mail Address rxk16664@nifty.ne.jp
My Web page http://tatsu.life.coocan.jp/index.html
GitHub project page https://github.com/Tatsu-syo/noMeiryoUI
------------------------------------------------------------------------
