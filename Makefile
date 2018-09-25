win: idict_win.cpp
	g++ idict_win.cpp -o idict -Os -s -m64 -std=c++14 -static-libgcc -static-libstdc++ -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -lws2_32

linux: idict_linux.cpp
	g++ idict_linux.cpp -o idict -Os -s -m64 -std=c++14 -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -ldl
	
macOs: idict_linux.cpp
	g++ -Os -m64 -std=c++14 idict_linux.cpp -o idict -lssl -lcrypto