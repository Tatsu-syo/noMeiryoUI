[English](README.md) | [简体中文](README_CN.md) | [繁體中文](README_TW.md) | 日本語

---
# Meiryo UIも大っきらい!!

Meiryo UIも大っきらい!!(海外名:No!! MeiryoUI)はWindowsの過去バージョンでできたシステムフォントを変更する機能を提供するソフトウェアです。

システムフォントを変更する機能がWindows 8で削除されたことから作成しました。

Windows 11 22H2以降ではMeiryo UIも大っきらい!!の動作に一部制限があります。

サポートは[discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions)で受け付けております。ご意見等ありましたらこちらをご利用ください。

## システム要件

- Windows 8以降
- Meiryo UIも大っきらい!!はWindows Vista以降で動作はします。ただし、互換性の確認はアクティブには行っておりません。

## インストール

> [!IMPORTANT]
> Meiryo UIも大っきらい!!ではレジストリを変更します。システムフォントの変更を行う前にレジストリのバックアップを推奨します。手順は以下となります。
>
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```
>
> 元に戻す場合は、管理者権限で以下を実行してください。
>
> ```
> reg restore "HKCU\Control Panel" .\reset_font.reg
> ```
>
> アーカイブ内の`utils\SAVEreg.bat`と`utils\RESTOREreg.bat`でも同じことができます。

- Zip形式のアーカイブを[GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest)からダウンロードしてください。
- アーカイブを展開して"noMeiryoUI.exe"を実行してください。

## 機能

- 以下の項目のフォントを個別に、または一括で変更できます:
  タイトルバー、アイコン、パレットタイトル、ヒント(ステータスバー)、メッセージボックス、メニュー
- プリセットメニューでWindows 8/8.1、10、11の初期状態のフォントに戻せます。
- フォント設定をファイルに保存・読み込みできます。
- 画面とヘルプは英語、日本語、韓国語、ポルトガル語(ブラジル)、簡体字中国語、繁体字中国語に対応しています。

## 既知の問題

- Meiryo UIも大っきらい!!は以下の箇所に対して効果がありません:
  - Windowsのタスクバー
  - 設定、ペイントなどプレインストールされたUWPアプリケーション
  - システムフォントを無視するアプリケーション

- Meiryo UIも大っきらい!!ではWindows 11 22H2以降のタイトルバーのフォントを変更できません
- Meiryo UIも大っきらい!!はシステムフォントを変更するためいくつかのアンチウイルスソフトでトロイの木馬やウイルスと判定される可能性があります。

## ドキュメント

ドキュメントは[wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki)の方に記載しております(英語)。
ヘルプファイル(`.chm`)もリリースのアーカイブに含まれています。ソースは[help](help)フォルダにあります。

## ビルド

ソースコードは[program](program)フォルダにあります。

1. Visual Studioを「C++によるデスクトップ開発」ワークロード付きでインストールします。
2. お使いのVisual Studioに合わせてソリューションファイルを開きます。

   | ソリューション | Visual Studio |
   | --- | --- |
   | `program\noMeiryoUI.sln` | 2022以降 (v143) |
   | `program\noMeiryoUI_2012.sln` | 2012 (v110) |
   | `program\noMeiryoUI_2008.sln` | 2008 |

3. Releaseをビルドします。実行ファイルは`program\bin\Release`(2022以降)または`program\bin_2012\Release`(2012)に生成されます。

配布用パッケージ(ポータブル版zip、インストーラー、言語パック)は[packaging](packaging/readme.md)フォルダのスクリプトで作成します。
新しいVisual Studioでビルドした実行ファイルはMicrosoft Defenderにウイルスと判定されるため、ポータブル版・インストーラー版にはVisual Studio 2012でのビルドを使用しています。

## 翻訳

画面(`lang`)とヘルプ(`help`)の翻訳を歓迎します。
作成方法は[Wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki/How-to-develop-and-translate)、提出方法は[CONTRIBUTING.md](CONTRIBUTING.md)(英語)を参照してください。

## ライセンス

Meiryo UIも大っきらい!!は[the MIT License](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE)でライセンスしております。
