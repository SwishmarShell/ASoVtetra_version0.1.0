#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <mutex>
#include "types.h"

#include "tcp.h"

// main.cpp 側のグローバル
extern FaceParams g_latestParams;
extern std::mutex g_paramsMutex;

// ------------------------------
// TCP 受信スレッド
// ------------------------------
void TcpRecvThread()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
        return;

    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9003);

    // ★ Android 側の IP に合わせる
    inet_pton(AF_INET, "192.168.137.1", &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        closesocket(sock);
        WSACleanup();
        return;
    }

    std::string buffer;
    char buf[256];

    while (true)
    {
        int len = recv(sock, buf, sizeof(buf) - 1, 0);
        if (len <= 0)
            break;

        buf[len] = '\0';
        buffer += buf;

        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos)
        {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);

            if (line.empty())
                continue;

            // ------------------------------
            // 軽量パース（OBS と同じ方式）
            // ------------------------------
            FaceParams p{};

            auto get_param = [&](const std::string& key, float& out)
                {
                    size_t kpos = line.find(key);
                    if (kpos == std::string::npos)
                        return;

                    size_t colon = line.find(':', kpos);
                    if (colon == std::string::npos)
                        return;

                    std::string num = line.substr(colon + 1);
                    size_t end = num.find_first_of(",}");
                    if (end != std::string::npos)
                        num = num.substr(0, end);

                    try { out = std::stof(num); }
                    catch (...) { out = 0.0f; }
                };

            get_param("jawOpen", p.jawOpen);
            get_param("eyeBlinkLeft", p.eyeBlinkLeft);
            get_param("eyeBlinkRight", p.eyeBlinkRight);
            get_param("yaw", p.yaw);
            get_param("browL", p.browL);
            get_param("browR", p.browR);

            // ------------------------------
            // 最新値だけ更新（mutex 保護）
            // ------------------------------
            {
                std::lock_guard<std::mutex> lock(g_paramsMutex);
                g_latestParams = p;
            }
        }
    }

    closesocket(sock);
    WSACleanup();
}