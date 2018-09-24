# iDict-cmd
<img src="https://img.shields.io/github/release/AntiSomnus/iDict-cmd.svg"/>

command line version of iDict for macOs&Windows&Linux

适用macOs,Windows和Linux的命令行版本iDict英语词典,包含330W词库以及来自权威词典的双语例句。

轻量级,包含所有依赖的可执行文件,下载即用。

同时推荐[微信小程序](https://github.com/AntiSomnus/iDict-weapp)版本，在移动端获取极佳体验

![macos_detail](/pics/macos_detail.png "macos_detail")

## Install

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
	g++ -Os -s -m64 -std=c++14 idict_win.cpp   -o idict -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -lws2_32
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
