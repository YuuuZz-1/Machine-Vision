# OpenCV 图像计数实验

使用 Visual Studio 2022 和 OpenCV 4.6.0 实现三个独立的 C++ 计数程序。

| 项目 | 独立源文件 | 示例结果 |
| --- | --- | --- |
| Paperclip Counting | Paperclip Counting/PaperclipCounting.cpp | 13 枚回形针 |
| CoinCounting | Paperclip Counting/CoinCounting.cpp | 10 枚硬币 |
| SmallHoleCounting | Paperclip Counting/SmallHoleCounting.cpp | 4 个完整小圆孔 |

## 开发环境

- Windows
- Visual Studio 2022，安装“使用 C++ 的桌面开发”、v143 工具集及 Windows SDK
- OpenCV 4.6.0 Windows x64 开发包

OpenCV 属性表位于本仓库的 OpenCV 文件夹。默认 SDK 路径为
D:\OpenCV_460\opencv\build。

如果 OpenCV 安装在其他位置，请创建名为 OpenCVRoot 的 Windows 环境变量，
将值设为 OpenCV 的 build 目录，然后重新启动 Visual Studio。
此目录应包含 include、x64\vc15\lib 和 x64\vc15\bin。

## 在 Visual Studio 中运行

1. 打开根目录中的 Paperclip Counting.sln。
2. 选择 Debug | x64 或 Release | x64。
3. 在解决方案资源管理器中，右键需要运行的项目，选择“设为启动项目”。
4. 按 Ctrl+F5 运行。

工程已配置命令行参数，从本仓库的 images 目录读取对应图片。
调试启动环境已配置 OpenCV DLL 目录。

## 单独使用源文件

三个 .cpp 文件各自包含 main()，可以分别放入配置好 OpenCV 的 C++ 项目。
单独运行时，可以使用第一个命令行参数指定图片路径，例如在 PowerShell 中执行：

~~~powershell
& ".\x64\Release\CoinCounting.exe" ".\images\Coin.png"
~~~

使用终端运行前，将 OpenCV 的 x64\vc15\bin 加入终端的 PATH。
没有传入图片参数时，源代码默认读取 D:\vs2023 下对应的原始图片。

## 算法

- 回形针计数使用 Otsu 阈值分割和连通域分析，过滤小噪点及图像黑边。
- 硬币计数使用中值滤波和霍夫圆变换，检测半径范围为 48–78 像素。
- 小圆孔计数使用中值滤波和霍夫圆变换，检测半径范围为 18–50 像素，并筛选完整位于图像内的圆。

圆半径范围按提供的示例图片设置，更换尺寸或拍摄条件后可能需要调整参数。

## 仓库内容

提交解决方案、三个项目、源文件、OpenCV 属性表、示例图片和本说明。
.gitignore 已配置 Visual Studio 缓存和编译输出的忽略规则。
OpenCV SDK 由开发环境单独安装。
