[English](README.md) | 简体中文 | [繁體中文](README_TW.md) | [日本語](README_JA.md)

---
# No!! MeiryoUI

No!! MeiryoUI（日文为“Meiryo UI も大っきらい!!”）重新启用了自定义 Windows 用户界面字体的功能，该功能自 Windows 8.1 起已被微软移除。

No!! MeiryoUI 在 Windows 11 22H2 及更高版本上存在部分功能局限性。

如果有任何想法或问题，请在 [discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions) 寻求支持。

## 系统要求

- Windows 8 或更新版本。
- 虽然 No!! MeiryoUI 应能在 Windows Vista 上工作，但并未积极测试其兼容性。

## 安装

> [!IMPORTANT]
> No!! MeiryoUI 会对注册表进行更改。建议在应用任何更改之前备份注册表。
>
> ```
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```
>
> 之后如需还原，请以管理员权限运行下面的命令。
>
> ```
> reg restore "HKCU\Control Panel" .\reset_font.reg
> ```
>
> 压缩包内的 `utils\SAVEreg.bat` 和 `utils\RESTOREreg.bat` 也能达到相同效果。

- 从 [GitHub Releases](https://github.com/Tatsu-syo/noMeiryoUI/releases/latest) 下载 zip 文件。
- 解压并运行“noMeiryoUI.exe”。

## 功能

- 可单独或一次性更改以下界面元素的字体：
  标题栏、图标、调色板标题、提示（状态栏）、消息框、菜单。
- 预设菜单：可将字体重置为 Windows 8/8.1、10、11 的默认值。
- 可将字体设置保存为文件，或从文件加载。
- 界面与帮助提供英语、日语、韩语、巴西葡萄牙语、简体中文和繁体中文。

## 已知问题

- No!! MeiryoUI 无法作用于：
  - Windows 任务栏。
  - 系统自带的 UWP 应用（例如“设置”、“画图”）。
  - 忽略系统字体设置的应用程序。

- No!! MeiryoUI 在 Windows 11 上无法修改标题栏字体。
- 因为 No!! MeiryoUI 会更改系统字体设置，可能会被一些杀毒软件检测为木马或病毒。

## 说明文档

您可以在 [Wiki](https://github.com/Tatsu-syo/noMeiryoUI/wiki) 上找到详细的说明文档。
发行压缩包内也附带帮助文件（`.chm`），其源文件位于 [help](help) 文件夹。

## 构建

源代码位于 [program](program) 文件夹。

1. 安装 Visual Studio，并勾选“使用 C++ 的桌面开发”工作负载。
2. 根据您的 Visual Studio 版本打开对应的解决方案文件。

   | 解决方案 | Visual Studio |
   | --- | --- |
   | `program\noMeiryoUI.sln` | 2022 或更新版本（v143） |
   | `program\noMeiryoUI_2012.sln` | 2012（v110） |
   | `program\noMeiryoUI_2008.sln` | 2008 |

3. 构建 Release 配置。可执行文件会生成在 `program\bin\Release`（2022 或更新版本）或 `program\bin_2012\Release`（2012）。

发行包（便携版 zip、安装程序和语言包）由 [packaging](packaging/readme.md) 文件夹内的脚本制作。
便携版与安装版使用 Visual Studio 2012 的构建结果，因为较新版本构建出的可执行文件会被 Microsoft Defender 误判为病毒。

## 翻译

欢迎提供界面（`lang`）和帮助（`help`）的翻译。
制作与提交方式请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)（英文）。

## 许可证

根据 [MIT 许可证](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE) 授权。
