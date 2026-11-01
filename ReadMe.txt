Meiryo UIも大っきらい!! source file package
By Tatsuhiko Syoji(Tatsu) 2005,2012-2025

ビルド方法・翻訳方法の詳細は README.md、Wiki、CONTRIBUTING.md を参照してください。
(README.md: ビルド、Wiki: 翻訳・ビルド詳細、CONTRIBUTING.md: 翻訳・コードの提交流程)

[必要となるプログラム]
・Visual Studio 2008/2012/2022以降(VC++)

[ファイル]
ReadMe.txt:本ファイルです。
README.md, README_CN.md, README_TW.md, README_JA.md : 各言語のREADMEです。
CONTRIBUTING.md : 翻訳・コード提供の手順です。
LICENSE : MITライセンスの条文です。

・programフォルダ配下のファイル
noMeiryoUI_2008.sln, noMeiryoUI_2008.vcproj : Visual Studio 2008用ソリューション・プロジェクトファイルです。
noMeiryoUI_2012.sln, noMeiryoUI_2012.vcxproj : Visual Studio 2012用ソリューション・プロジェクトファイルです。
noMeiryoUI.sln, noMeiryoUI.vcxproj : Visual Studio 2022以降用ソリューション・プロジェクトファイルです。
TWRフォルダ配下のファイル : GUI構築に使用したファイルです。
その他のファイル : Meiryo UIも大っきらい!!自身のソースファイルです。

・helpフォルダ配下のファイル
各言語のフォルダ(ja-jp, en-us, zh-cn, zh-tw, pt-br, ko-kr)にヘルプファイルのソースファイルがあります。

・langフォルダ配下のファイル
default.lng : 言語ファイルのサンプルを兼ねた、言語に合う言語ファイルがないときに使用される言語ファイルです。
en.lng, ja-JP.lng, ko-KR.lng, pt-BR.lng, zh-CN.lng, zh-TW.lng : 各言語の言語ファイルです。

・packagingフォルダ配下のファイル
ポータブル版・インストーラー版・言語パックの作成用ファイルです。手順はpackaging\readme.mdを参照してください。

・utilフォルダ配下のファイル
SAVEreg.bat, RESTOREreg.bat : レジストリのバックアップ・復元用バッチファイルです。
localetest.c, localetest.exe : 現在のWindowsのロケール名を表示するプログラムです。

[使い方]
・プログラムのビルド
1.まず、本アーカイブをディレクトリ付きで展開します。
2.programフォルダのnoMeiryoUI_2008.sln(Visual Studio 2008)、
　noMeiryoUI_2012.sln(Visual Studio 2012)、
　noMeiryoUI.sln(Visual Studio 2022以降)のいずれかを開きます。
3.Releaseをビルドすると実行ファイルがprogram\bin\Release
　(2012版はprogram\bin_2012\Release)に生成されます。

・翻訳
言語ファイル(lang)・ヘルプファイル(help)の翻訳方法はWiki( https://github.com/Tatsu-syo/noMeiryoUI/wiki/How-to-develop-and-translate )、提出方法はCONTRIBUTING.mdを参照してください。

[ライセンス]
MITライセンス( http://opensource.org/licenses/mit-license.php )に従います。

[作者]
庄子 達彦(Tatsu)
E-mail Address tatsu_syo@nifty.com
My Web page http://tatsu.life.coocan.jp/index.html
GitHub project page https://github.com/Tatsu-syo/noMeiryoUI
------------------------------------------------------------------------
