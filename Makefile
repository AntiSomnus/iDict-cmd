win: idict_win.cpp
	g++ -Os -s -m64 -std=c++14 idict_win.cpp   -o idict -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -lws2_32

linux: idict_linux.cpp
	g++ -Os -s -m64 -std=c++14 idict_linux.cpp -o idict -Wl,-Bstatic -lssl -lcrypto -Wl,-Bdynamic -ldl
