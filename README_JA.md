[En](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/README.md) | [Cn](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/README_CN.md) | Jp

# Meiryo UIも大っきらい!!

Meiryo UIも大っきらい!!(海外名:No!! MeiryoUI)はWindowsの過去バージョンでできたシステムフォントを変更する機能を提供するソフトウェアです、

システムフォントを変更する機能がWindows 8で削除されたことから作成しました。

Windows 11 22H2以降ではMeiryo UIも大っきらい!!の動作に一部制限があります、

サポートは[discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions)で受け付けております。ご意見等ありましたらこちらをご利用ください。

## システム要件

- Windows 8以降
- Meiryo UIも大っきらい!!はWindows Vista以降で動作はします。検証用のため、互換性の確認はアクティブには行っておりません。

## インストール

> [重要!]  
> Meiryo UIも大っきらい!!ではレジストリを変更します。システムフォントの変更を行う前にレジストリのバックアップを推奨します。手順は以下となります。
>
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```

- Zip形式のアーカイブを[GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest)からダウンロードしてください。
- アーカイブを展開して"noMeiryoUI.exe"を実行してください。

## 既知の問題

- Meiryo UIも大っきらい!!は以下の箇所にで効果がありません:
  - Windowsのタスクバー
  - 設定、ペイントなどプレインストールされたUWPアプリケーション
  - システムフォントを無視するアプリケーション

- Meiryo UIも大っきらい!!ではWindows 11 22H2以降のタイトルバーのフォントを変更できません
- Meiryo UIも大っきらい!!はシステムフォントを変更するためいくつかのアンチウイルスソフトでトロイの木馬やウイルスと判定される可能性があります。

## ドキュメント

ドキュメントは[wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki)の方に記載しております(英語).

## ライセンス

Meiryo UIも大っきらい!!は[the MIT License](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE)でライセンススしております.
