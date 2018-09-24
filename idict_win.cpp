#include "json.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <openssl/ssl.h>

#define DEFAULT_BUFLEN 16384
using json = nlohmann::json;
using std::string;
std::map<string, string> sourceMap = { {"CAMBRIDGE", "剑桥高阶英汉双解词典"}, {"LONGMAN", "朗文当代高级英语词典"}, {"COLLINS", "柯林斯英汉双解大词典"}, {"ONLINE", "金山词霸"} };
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
short backgroundColor;
int maxSentence = 2;

bool GetColor(short &ret) {
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
		return false;
	ret = info.wAttributes;
	return true;
}
string httpsRuest(const char *host, const char *message)
{
	char const *portno = "443";
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(host, portno, &hints, &result);

	SOCKET ConnectSocket = INVALID_SOCKET;
	ptr = result;
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	freeaddrinfo(result);

	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	SSL_library_init();
	SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
	SSL *conn = SSL_new(ssl_ctx);
	SSL_set_fd(conn, ConnectSocket);
	SSL_connect(conn);

	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN] = { '0' };
	SSL_write(conn, message, (int)strlen(message));
	string response{ "" };
	int iResult;
	do
	{
		do
		{
			iResult = SSL_read(conn, recvbuf, recvbuflen - 1);
			response += recvbuf;
			memset(recvbuf, 0, sizeof(recvbuf));
		} while (SSL_pending(conn) > 0);
	} while (iResult > 0);

	SSL_free(conn);
	iResult = shutdown(ConnectSocket, SD_SEND);
	return response;
}
void parseBrief(json brief)
{
	if (brief["lemma"].find("relation") != brief["lemma"].end())
	{
		SetConsoleTextAttribute(hConsole, 12 | backgroundColor);
		printf("%s: ", brief["wordOut"].get<string>().c_str());
		SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
		printf("%s 的 %s\n", brief["lemma"]["lemma"].get<string>().c_str(), brief["lemma"]["relation"].get<string>().c_str());
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 12 | backgroundColor);
		printf("%s:\n", brief["wordOut"].get<string>().c_str());
	}
	if (brief.find("usPron") != brief.end() || brief.find("ukPron") != brief.end())
	{
		// SetConsoleTextAttribute(hConsole, 10 | backgroundColor);
		// printf("  音标\n");
		if (brief.find("usPron") != brief.end())
		{
			SetConsoleTextAttribute(hConsole, 14 | backgroundColor);
			printf("    美音 ");
			SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
			printf("/%-2s/", brief["usPron"]["ps"].get<string>().c_str());
		}
		if (brief.find("ukPron") != brief.end())
		{
			SetConsoleTextAttribute(hConsole, 14 | backgroundColor);
			printf("    英音 ");
			SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
			printf("/%-2s/", brief["ukPron"]["ps"].get<string>().c_str());
		}
		printf("\n");
	}
	if (brief.find("chnDefinitions") != brief.end())
	{
		auto chn = brief["chnDefinitions"];
		SetConsoleTextAttribute(hConsole, 10 | backgroundColor);
		printf("中文释义\n");
		for (auto &def : chn)
		{
			if (def.find("pos") != def.end())
			{
				SetConsoleTextAttribute(hConsole, 14 | backgroundColor);
				printf("    %-8s", def["pos"].get<string>().c_str());
				SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
				printf("%s\n", def["meaning"].get<string>().c_str());
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
				printf("            %s\n", def["meaning"].get<string>().c_str());
			}
		}
	}
	if (brief.find("engDefinitions") != brief.end())
	{
		auto chn = brief["engDefinitions"];
		SetConsoleTextAttribute(hConsole, 10 | backgroundColor);
		printf("英文释义\n");
		for (auto &def : chn)
		{
			if (def.find("pos") != def.end())
			{
				SetConsoleTextAttribute(hConsole, 14 | backgroundColor);
				printf("    %-8s", def["pos"].get<string>().c_str());
				SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
				printf("%s\n", def["meaning"].get<string>().c_str());
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
				printf("            %s\n", def["meaning"].get<string>().c_str());
			}
		}
	}
}
string parseSource(json sentenceList)
{
	if (sentenceList.find("source") != sentenceList.end())
		return sourceMap[sentenceList["source"].get<string>()];
	else
		return "牛津高阶英汉双解词典";
}
void parseDetail(json detail)
{
	parseBrief(detail["wordBrief"]);
	if (detail.find("sentenceLists") != detail.end())
	{
		auto sentenceLists = detail["sentenceLists"];
		SetConsoleTextAttribute(hConsole, 10 | backgroundColor);
		printf("双语例句\n");
		for (auto &sentenceList : sentenceLists)
		{
			SetConsoleTextAttribute(hConsole, 14 | backgroundColor);
			printf("    %s\n", parseSource(sentenceList).c_str());
			SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
			int count = 0;
			for (auto &sentence : sentenceList["sentences"])
			{
				if (count < maxSentence)
				{
					count++;
					printf("        %d. %s\n", count, sentence["eng"].get<string>().c_str());
					printf("           %s\n", sentence["chn"].get<string>().c_str());
				}
				else
					break;
			}
		}
	}
}
int main(int argc, char *argv[])
{
	if (GetConsoleCP() != 65001)
	{
		SetConsoleCP(65001);
		SetConsoleOutputCP(65001);
	}
	GetColor(backgroundColor);
	backgroundColor &= 240;
	char const *host = "www.ireading.site";
	char const *message_fmt = "GET /word/detail/?json=true&tag=false&word=%s HTTP/1.0\r\n\r\n";
	char message[1024];
	bool isBrief = true;

	if (argc < 2)
	{
		puts("Parameter: <word>");
		exit(0);
	}
	string cat_input{ "" };
	bool isPreviousDetail = false;
	for (int i = 1; i < argc; i++)
	{
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--detail"))
		{ //argv[i] == "-d"|"--detail"
			isBrief = false;
			isPreviousDetail = true;
		}
		else if (isPreviousDetail) {
			isPreviousDetail = false;
			maxSentence = atoi(argv[i]);
		}
		else
		{ //argv[i] != "-d"|"--detail"
			if (i != 1)
			{
				cat_input += "%20";
			}
			cat_input += argv[i];
		}
	}
	sprintf(message, message_fmt, cat_input.c_str());

	string response = httpsRuest(host, message);
	try {
		response = response.substr(response.find('{'));
		if (isBrief)
		{ //brief
			parseBrief(json::parse(response)["wordBrief"]);
		}
		else
		{
			parseDetail(json::parse(response));
		}
	}
	catch (std::out_of_range) {
		SetConsoleTextAttribute(hConsole, 15 | backgroundColor);
		printf("该单词不存在");
	}


	return 0;
}
