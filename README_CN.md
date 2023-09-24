# No!! MeiryoUI 

No!! MeiryoUI重新启用了自定义Windows用户界面字体的功能，该功能自Windows 8.1以来已被删除。

No!! MeiryoUI 在Windows 11 22H2上有一些局限性，

如果您有一些想法或问题，请点击参[discussions](https://github.com/tatsu-syo/nomeiryoui/discussions)。

## 使用需求

 -  Windows 8 或以上。
 - 虽然NoMeiryoUI能在Windows Vista上工作，但并不会积极测试Bug并确保兼容性，建议使用系统设置。

## 使用注意

> [!IMPORTANT]  
> No!! MeiryoUI 会对注册表进行更改。建议在应用任何更改之前备份注册表。
> 
> ```
> 注册表储存在 "HKCU\Control Panel" .\reset_font.reg /y
> ```

 - 从[GitHub Releases](https://github.com/tatsu-syo/nomeiryoui/releases/latest)下载zip文件。
 - 全部解压至一个文件夹并运行"nomeiryoui.exe".。

## 已知问题

- No!! MeiryoUI 无法工作在：
    -窗口任务栏。
    -预安装的 UWP 应用（例如"设置"、"画图"等）。
    -忽略系统字体设置的应用程序。
    - No!! MeiryoUI 无法修改Windows 11的标题字体。

## 说明文档

您可以在[Wiki](https://github.com/tatsu-syo/nomeiryoui/wiki)上找到详细的说明文档。

## 许可证书

使用[MIT License](https://github.com/tatsu-syo/nomeiryoui/blob/master/license)。
