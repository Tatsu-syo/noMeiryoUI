[English](README.md) | 简体中文 | [日本語](README_JA.md)

---
# No!! MeiryoUI 

No!! MeiryoUI 重新启用了自定义 Windows 用户界面字体的功能，该功能自 Windows 8.1 以来已被删除。

No!! MeiryoUI 在 Windows 11 22H2 上有一些局限性，

如果有任何想法或问题，请在 [discussions](https://github.com/Tatsu-syo/noMeiryoUI/discussions) 寻求支持。

## 使用需求

- Windows 8 或更新版本。
 - 虽然 No!! MeiryoUI 能在 Windows Vista 上工作，但并未积极测试其兼容性。

## 使用注意

> [!IMPORTANT]  
> No!! MeiryoUI 会对注册表进行更改。建议在应用任何更改之前备份注册表。
> 
> ``` batchfile
> reg save "HKCU\Control Panel" .\reset_font.reg /y
> ```

 - 从 [GitHub Releases](https://github.com/tatsu-syo/nomeiryoui/releases/latest) 下载zip文件。
 - 解压并运行"nomeiryoui.exe".。

## 已知问题

- No!! MeiryoUI 无法工作在：
    - Windows 任务栏。
    - 系统自带的 UWP 应用（例如“设置”、“画图”）。
    - 忽略系统字体设置的应用程序。
    
- No!! MeiryoUI 在 Windows 11 上无法修改标题栏字体。
- No!! MeiryoUI 可能会被一些杀毒软件检测为木马/病毒软件，因为 No!! MeiryoUI 会更改系统字体设置。

## 说明文档

您可以在 [Wiki](https://github.com/tatsu-syo/nomeiryoui/wiki) 上找到详细的说明文档。

## 许可证书

根据 [MIT LICENSE](https://github.com/Tatsu-syo/noMeiryoUI/blob/master/LICENSE) 授权。
