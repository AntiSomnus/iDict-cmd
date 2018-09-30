<!-- TOC -->

- [iDict-cmd](#idict-cmd)
    - [Install](#install)
    - [Usage](#usage)
    - [Example](#example)
    - [Color (Beta in win64_python version)](#color-beta-in-win64python-version)
    - [Sentences from news (Beta in win64_python version)](#sentences-from-news-beta-in-win64python-version)
    - [Compile on your own](#compile-on-your-own)

<!-- /TOC -->
# iDict-cmd
<img src="https://img.shields.io/github/release/AntiSomnus/iDict-cmd.svg"/>

command line version of iDict for macOs&Windows&Linux

适用macOs, Windows(64位)和Linux的命令行版本iDict英语词典,包含330W词库以及来自权威词典的双语例句。

轻量级,包含所有依赖的可执行文件,下载即用。

同时推荐[微信小程序](https://github.com/AntiSomnus/iDict-weapp)版本，在移动端获取极佳体验

![macos_detail](/pics/macos_detail.png "macos_detail")

## Install
请去[Release](https://github.com/AntiSomnus/iDict-cmd/releases/latest)页面下载对应的可执行文件
- Linux

    1. `wget https://github.com/AntiSomnus/iDict-cmd/releases/download/[latest-release-number]/idict_linux && chmod a+x idict_linux`

        Make sure to change [latest-release-number] to the number of release, e.g `2.0`

        Or you can just go to [Release](https://github.com/AntiSomnus/iDict-cmd/releases/latest) page to download the latest version.

    2. add the location to the path

        `ln -s /location/to/idict_linux /usr/local/bin/idict`
    3. `idict welcome` `idict welcome -d` `idict welcome -d 3`

- Windows

    1. Just go to [Release](https://github.com/AntiSomnus/iDict-cmd/releases/latest) page to download the latest version.
    2. add the location to the path
    3. `idict welcome` `idict welcome -d` `idict welcome -d 3`

- macOs

    macOs上使用需要brew安装openssl

    1. `wget https://github.com/AntiSomnus/iDict-cmd/releases/download/[latest-release-number]/idict_macos && chmod a+x idict_macos`

        Make sure to change [latest-release-number] to the number of release, e.g `2.0`

        Or you can just go to [Release](https://github.com/AntiSomnus/iDict-cmd/releases/latest) page to download the latest version.
    2. make sure you have `openssl` installed

        `brew install openssl`

    3. add the location to the path

        `ln -s /location/to/idict_macos /usr/local/bin/idict`
    4. `idict welcome` `idict welcome -d` `idict welcome -d 3`

## Usage
```
idict word [-d|--detail [max-sentences-per-dict] ]
word            the word you want to query            你想要查询的单词,默认不包含例句
-d | --detail   get the detailed info of the word     获取单词详细释义，包含例句
                max-sentences-per-dict(default=2)     显示的例句数量，默认为2
```
## Example
```
idict welcome
idict welcome -d
idict welcome -d 1
```
![windows_brief](/pics/windows_brief.png "windows_brief")

![linux_detail](/pics/linux_detail.png "linux_detail")

![windows_detail](/pics/windows_detail.png "windows_detail")

## Color (Beta in win64_python version)
修改`color.ini`即可修改颜色

New python version will give you the flexibility to change color configuration.

Just change the `color.ini` in the folder of `win64_python` Release

The color table is below
```
BLACK           = 30
RED             = 31
GREEN           = 32
YELLOW          = 33
BLUE            = 34
MAGENTA         = 35
CYAN            = 36
WHITE           = 37
RESET           = 39

LIGHTBLACK      = 90
LIGHTRED        = 91
LIGHTGREEN      = 92
LIGHTYELLOW     = 93
LIGHTBLUE       = 94
LIGHTMAGENTA    = 95
LIGHTCYAN       = 96
LIGHTWHITE      = 97
```

default color configuration is
```
[COLOR]
word_color = 91
section_color = 92
hint_color = 93
text_color = 97
source_color = 90
```
## Sentences from news (Beta in win64_python version)
可以获得来自国外权威新闻媒体对于该单词的用法，在`win64_python`版本中使用`-n`即可开启，如`idict test -n`或`idict test -n -d`

New feature that provides sentences from news by the api of `vocabulary.com`
## Compile on your own
- Linux
    ```
    g++ -Os -s -m64 -std=c++14 idict_linux.cpp -o idict -Wl,-static -lssl -lcrypto -Wl,-Bdynamic -ldl
    ```
    if you have the error of `undefined reference to 'pthread_getspecific'`, you might need to add `-pthread`
    ```
    g++ -Os -s -m64 -std=c++14 idict_linux.cpp -o idict -Wl,-static -lssl -lcrypto -Wl,-Bdynamic -ldl -pthread
    ```
- Windows
    ```
	g++ idict_win.cpp -o idict -Os -s -m64 -std=c++14 -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -lws2_32
    ```
- macOs
    In order to use it properly, you must install openssl from brew first.


    1. ` brew install openssl`
    2. For compilers to find openssl you **must** set:
        ```
        export LD_LIBRARY_PATH=/usr/local/opt/openssl/lib:"${LD_LIBRARY_PATH}"
        export CPATH=/usr/local/opt/openssl/include:"${CPATH}"
        ```
    3. `https://github.com/AntiSomnus/iDict-cmd.git`
    4. `cd iDict-cmd`
    5. Compile
        ```
        g++ -Os -m64 -std=c++14 idict_linux.cpp -o idict -lssl -lcrypto
        ```
