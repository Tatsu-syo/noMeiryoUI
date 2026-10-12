[English](README.md) | [简体中文](README_CN.md) | 繁體中文 | [日本語](README_JA.md)

---
# No!! MeiryoUI

No!! MeiryoUI（日文為「Meiryo UI も大っきらい!!」）重新提供自訂 Windows 使用者介面字型的功能，該功能自 Windows 8.1 起已被微軟移除。

No!! MeiryoUI 在 Windows 11 22H2 及更新版本上存在部分功能限制。

如果有任何想法或問題，請至 [discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions) 尋求支援。

## 系統需求

- Windows 8 或更新版本。
- No!! MeiryoUI 應可在 Windows Vista 上執行，但並未主動測試其相容性。

## 安裝

> [!IMPORTANT]
> No!! MeiryoUI 會變更登錄檔。建議在套用任何變更之前先備份登錄檔。
>
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```
>
> 之後若要還原，請以系統管理員權限執行下列命令。
>
> ```
> reg restore "HKCU\Control Panel" .\reset_font.reg
> ```
>
> 壓縮檔內的 `utils\SAVEreg.bat` 與 `utils\RESTOREreg.bat` 也能達到相同效果。

- 從 [GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest) 下載 zip 壓縮檔。
- 解壓縮後執行「noMeiryoUI.exe」。

## 功能

- 可個別或一次變更下列介面項目的字型：
  標題列、圖示、調色盤標題、提示（狀態列）、訊息方塊、功能表。
- 預設設定功能表：可將字型重設為 Windows 8/8.1、10、11 的預設值。
- 可將字型設定儲存為檔案，或從檔案載入。
- 介面與說明提供英文、日文、韓文、巴西葡萄牙文、簡體中文與繁體中文。

## 已知限制

- No!! MeiryoUI 對下列項目無效：
  - Windows 工作列。
  - 內建的 UWP 應用程式（例如「設定」、「小畫家」）。
  - 忽略系統字型設定的應用程式。

- No!! MeiryoUI 無法變更 Windows 11 的標題列字型。
- 由於 No!! MeiryoUI 會變更系統字型設定，部分防毒軟體可能將其誤判為木馬或病毒。

## 說明文件

詳細說明請參閱 [Wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki)。
發行壓縮檔內也附有說明檔（`.chm`），其原始檔位於 [help](help) 資料夾。

## 建置

原始碼位於 [program](program) 資料夾。

1. 安裝 Visual Studio，並勾選「使用 C++ 的桌面開發」工作負載。
2. 依您的 Visual Studio 版本開啟對應的方案檔。

   | 方案檔 | Visual Studio |
   | --- | --- |
   | `program\noMeiryoUI.sln` | 2022 或更新版本（v143） |
   | `program\noMeiryoUI_2012.sln` | 2012（v110） |
   | `program\noMeiryoUI_2008.sln` | 2008 |

3. 建置 Release 組態。執行檔會產生在 `program\bin\Release`（2022 或更新版本）或 `program\bin_2012\Release`（2012）。

發行用的封裝（可攜版 zip、安裝程式與語言包）由 [packaging](packaging/readme.md) 資料夾內的指令碼製作。
可攜版與安裝版使用 Visual Studio 2012 的建置結果，因為較新版本建置出的執行檔會被 Microsoft Defender 誤判為病毒。

## 翻譯

歡迎提供介面（`lang`）與說明（`help`）的翻譯。
製作與提交方式請參閱 [CONTRIBUTING.md](CONTRIBUTING.md)（英文）。

## 授權

依 [MIT 授權條款](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE) 授權。
