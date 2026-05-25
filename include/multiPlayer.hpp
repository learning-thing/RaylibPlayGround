//
// Created by benji on 23.05.26.
//

#ifndef RAYLIBPLAYGROUND_MULTIPLAYER_HPP
#define RAYLIBPLAYGROUND_MULTIPLAYER_HPP

#include <assert.h>
#include <isteamnetworkingsockets.h>
#include <isteamnetworkingutils.h>
#include <map>
#include <string>
#include <cstdint>
#include <ranges>
#include <bits/this_thread_sleep.h>

#include "steamnetworkingsockets.h"

using namespace std::chrono_literals;

static void InitSteamDatagramConnectionSockets() {
    SteamDatagramErrMsg errMsg;
    if (!GameNetworkingSockets_Init(nullptr, errMsg)) {
        printf("Failed to initialize SteamGameNetworkingSockets lib\n");
    }
}
static void ShutdownSteamDatagramConnectionSockets() {
    std::this_thread::sleep_for(250ms);
    GameNetworkingSockets_Kill();
}

class Client {
public:
    int m_nMsgs = 0;
    std::string m_sMsg;
    void Connect(const SteamNetworkingIPAddr &serverAddr) {
        InitSteamDatagramConnectionSockets();
        m_pInterface = SteamNetworkingSockets();

        char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
        serverAddr.ToString(szAddr, sizeof(szAddr), true);
        //printf("Connectiong to server at %s", szAddr);
        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, reinterpret_cast<void*>(SteamConnectionStatusChangedCallback));
        m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 1, &opt);
        if (m_hConnection == k_HSteamNetConnection_Invalid) { printf("Failed to create Connection\n"); return; }
    }
    void Connect(const char *addr) {
        SteamNetworkingIPAddr serverAddr;
        serverAddr.Clear();
        serverAddr.ParseString(addr);

        InitSteamDatagramConnectionSockets();
        m_pInterface = SteamNetworkingSockets();

        char szAddr[SteamNetworkingIPAddr::k_cchMaxString];
        serverAddr.ToString(szAddr, sizeof(szAddr), true);
        //printf("Connectiong to server at %s", szAddr);
        SteamNetworkingConfigValue_t opt;
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, reinterpret_cast<void*>(SteamConnectionStatusChangedCallback));
        m_hConnection = m_pInterface->ConnectByIPAddress(serverAddr, 1, &opt);
        if (m_hConnection == k_HSteamNetConnection_Invalid) { printf("Failed to create Connection\n"); return; }
    }
    void Send(const std::string& msg) const {
        m_pInterface->SendMessageToConnection(m_hConnection, msg.c_str(), static_cast<uint32>(msg.length()), k_nSteamNetworkingSend_Reliable, nullptr);
    }
    void Poll() {
        PollIncomingMessages();
        PollConnectionStateChanged();
        std::this_thread::sleep_for( 10ms );
    }
    void Disconnect() const {
        m_pInterface->CloseConnection(m_hConnection, 0, "Disconnect requested.", 1);
    }
    void Cleanup() const {
        Disconnect();
        ShutdownSteamDatagramConnectionSockets();
    }
    std::string getMessage() {
        unreadMessage = false;
        std::string ret = m_sMsg;
        m_sMsg.clear();
        return ret;
    }
    [[nodiscard]] bool messageAvailable() const {
        return unreadMessage;
    }
private:
    bool unreadMessage = false;
    HSteamNetConnection m_hConnection;
    ISteamNetworkingSockets *m_pInterface = nullptr;
    void PollIncomingMessages() {
        while (true) {
            ISteamNetworkingMessage *pIncomingMsg = nullptr;
            m_nMsgs = m_pInterface->ReceiveMessagesOnConnection(m_hConnection, &pIncomingMsg, 1);
            if (!m_nMsgs) break;
            //std::clog << "Got " << m_nMsgs << " messages" << std::endl;
            unreadMessage = true;
            if (m_nMsgs < 0) { printf("Critical error while receiving messages"); return; }

            /*
            fwrite( pIncomingMsg->m_pData, 1, pIncomingMsg->m_cbSize, stdout );
            fputc( '\n', stdout );
            */
            m_sMsg.assign(static_cast<const char *>(pIncomingMsg->m_pData), pIncomingMsg->m_cbSize);

            pIncomingMsg->Release();
        }
    }
    void OnSteamnetCOnnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pInfo) {

    }

    static Client *s_pCallbackInstance;
    static void SteamConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *pInfo) {
        s_pCallbackInstance->OnSteamnetCOnnectionStatusChanged(pInfo);
    }
    void PollConnectionStateChanged() {
        s_pCallbackInstance = this;
        m_pInterface->RunCallbacks();
    }
};

class Server {
private:
    bool unreadMessage = false;
    HSteamListenSocket m_hListenSock;
    HSteamNetPollGroup m_hPollGroup;
    ISteamNetworkingSockets *m_pInterface = nullptr;
    struct ClientData_t { size_t id; std::string m_sUsername; };
    std::map<HSteamNetConnection, ClientData_t> m_pClientMap;

    void SendStringToClient(const HSteamNetConnection& conn, const char *str) const {
        m_pInterface->SendMessageToConnection(conn, str, static_cast<uint32>(strlen(str)), k_nSteamNetworkingSend_Reliable, nullptr);
    }
    void PollMessages() {
        char tmp[1024];
        while (true) {//While messages arein the queue
            ISteamNetworkingMessage *pIncomingMessage = nullptr;
            m_nMsgs = m_pInterface->ReceiveMessagesOnPollGroup(m_hPollGroup, &pIncomingMessage, 1);
            if (!m_nMsgs) break;//break if no messages left
            //std::clog << "Got " << m_nMsgs << " messages" << std::endl;
            unreadMessage = true;
            if (m_nMsgs < 0) printf("Something unexpected went wrong");
            assert(m_nMsgs == 1 && pIncomingMessage);
            auto itClient = m_pClientMap.find(pIncomingMessage->m_conn);
            assert(itClient != m_pClientMap.end());

            m_sMsg.assign(static_cast<const char *>(pIncomingMessage->m_pData), pIncomingMessage->m_cbSize);
            //std::cout << "Message: " << m_sMsg << std::endl;
            pIncomingMessage->Release();
        }
    }
    void setClientUsername(const HSteamNetConnection& hConn, const char *name) {
        m_pClientMap[hConn].m_sUsername = name;
        m_pInterface->SetConnectionName(hConn, name);
    }
    void OnSteamNetConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *pInfo) {
        char tmp[1024];
        switch (pInfo->m_info.m_eState) {
            case k_ESteamNetworkingConnectionState_None:
                break;
            case k_ESteamNetworkingConnectionState_ClosedByPeer:
            case k_ESteamNetworkingConnectionState_ProblemDetectedLocally: {
                //Check Previous connection status
                if (pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connected) {
                    const auto itClient = m_pClientMap.find(pInfo->m_hConn);
                    assert(itClient != m_pClientMap.end());

                    const char *pszDebugLogAction;
                    if (pInfo->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally) {
                        pszDebugLogAction = "problem detected locally";
                        sprintf(tmp, "Alas, %s hath fallen into Shadow", itClient->second.m_sUsername.c_str());
                    } else {
                        pszDebugLogAction = "closed by peer";
                        sprintf(tmp, "Alas, %s hath departed", itClient->second.m_sUsername.c_str());
                    }
                    printf("Connection %s %s, reason %d: %s\n",
                        pInfo->m_info.m_szConnectionDescription,
                        pszDebugLogAction,
                        pInfo->m_info.m_eEndReason,
                        pInfo->m_info.m_szEndDebug
                    );

                    m_pClientMap.erase(itClient);

                    SendStringAllClients(tmp);
                } else {
                    assert(pInfo->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
                }

                m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                break;
            }
            case k_ESteamNetworkingConnectionState_Connecting: {
                assert(!m_pClientMap.contains(pInfo->m_hConn));//Make sure it's new
                printf("Connection request from %s\n", pInfo->m_info.m_szConnectionDescription);

                if (m_pInterface->AcceptConnection(pInfo->m_hConn) != k_EResultOK) {
                    m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                    printf("Can't accept connection! (already closed?)\n");
                    break;
                }
                // assign poll group
                if (!m_pInterface->SetConnectionPollGroup(pInfo->m_hConn, m_hPollGroup)) {
                    m_pInterface->CloseConnection(pInfo->m_hConn, 0, nullptr, false);
                    printf("Failed to set poll group wtf?\n");
                    break;
                }
                char nick[64];
                sprintf( nick, "Player%lu", m_pClientMap.size());

                // Send them a welcome message
                sprintf( tmp, "Welcome:%s", nick );
                SendStringToClient( pInfo->m_hConn, tmp );

                m_pClientMap[pInfo->m_hConn] = {m_pClientMap.size(), nick};
                break;
            }
            case k_ESteamNetworkingConnectionState_Connected:
                //ignore since we have the callback
                break;
            default:
                printf("Something happened, idk tbh\n");
                break;
        }
    }
    static Server *s_pCallbackInstance;
    static void SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *pInfo) {
        s_pCallbackInstance->OnSteamNetConnectionStatusChanged(pInfo);
    }
    void PollConnectionStateChanges() {
        s_pCallbackInstance = this;
        m_pInterface->RunCallbacks();
    }
public:
    std::string m_sMsg;
    int m_nMsgs = 0;
    void SendStringAllClients(const char *str, const HSteamNetConnection except = k_HSteamNetConnection_Invalid) const {
        for (const auto &key: m_pClientMap | std::views::keys)
            if (key != except) SendStringToClient(key, str);
    }
    void Initialize(const uint16 nPort) {
        InitSteamDatagramConnectionSockets();

        m_pInterface = SteamNetworkingSockets();

        SteamNetworkingIPAddr localAddr;
        localAddr.Clear();
        localAddr.m_port = nPort;

        SteamNetworkingConfigValue_t opt;
        //Set callback
        opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged, reinterpret_cast<void*>(SteamNetConnectionStatusChangedCallback));

        m_hListenSock = m_pInterface->CreateListenSocketIP(localAddr, 1, &opt);
        if (m_hListenSock == k_HSteamListenSocket_Invalid) { printf("Failed to listen on port %d\n", nPort); return; }
        m_hPollGroup = m_pInterface->CreatePollGroup();
        if (m_hPollGroup == k_HSteamNetPollGroup_Invalid)  { printf("Pollgroup invalid\n"); return; }
        printf("Server listening on port %d\n", nPort);
    }
    void Listen() {//put this inside a loop
        PollMessages();
        PollConnectionStateChanges();
        std::this_thread::sleep_for( 10ms );
    }
    std::string getMessage() {
        unreadMessage = false;
        std::string ret = m_sMsg;
        m_sMsg.clear();
        return ret;
    }
    bool messageAvailable() {
        return unreadMessage;
    }
    void Cleanup() {
        printf("Closing Server and connections\n");
        for (const auto &client: m_pClientMap | std::views::keys) {
            SendStringToClient(client, "Server is shutting down. Goodbye");
            m_pInterface->CloseConnection(client, 0, "Server shutting down now.", true);
        }
        ShutdownSteamDatagramConnectionSockets();
    }
    int IncomingMessageCount() const {
        return m_nMsgs;
    }
};

Client *Client::s_pCallbackInstance = nullptr;
Server *Server::s_pCallbackInstance = nullptr;

enum networkingMode_t {
    NETWORK_MODE_HOST,
    NETWORK_MODE_CLIENT,
    NETWORK_MODE_SINGLEPLAYER
};

class NetworkManager {
private:
    networkingMode_t mode = NETWORK_MODE_SINGLEPLAYER;
    Client client_;
    Server server_;
public:
    void host(int port) {

    }
    void connect(const char *address, int port) {

    }
};

#endif //RAYLIBPLAYGROUND_MULTIPLAYER_HPP
