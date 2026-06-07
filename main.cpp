#include <iostream>
#include <mujs.h>
#include <filesystem>
#include <fstream>
#include <raylib.h>
#include <unordered_map>
#include <jsFuncs.hpp>
#include <chrono>
#include <multiPlayer.hpp>
#include <string>
#include <templates.hpp>

using namespace std::chrono_literals;

int main(const int argc, char **argv) {
    js_State *runtime = js_newstate(nullptr, nullptr, 0);
    setupRaylibFuncs(runtime);
    std::string scriptPath = "script.js";
    if (argc > 1) scriptPath = argv[1];

    //Create a script template
    if (!std::filesystem::exists(scriptPath)) create_script_template(scriptPath.c_str());
    //Create the function declerations
    if (!std::filesystem::exists(".d.ts")) create_dts_template();
    if (js_dofile(runtime, scriptPath.c_str())) {
        std::cerr << "Error while running script\n";
        return -1;
    }

    //Function on init
    //App start, Raylib not initilaized
    js_dostring(runtime, "onStart();");
#ifdef multiplayer
    if (!g_headLessMode)
#endif
    InitWindow(1080, 720, "Hello world");

    // Onready function (after window creation)
    js_dostring(runtime, "onReady();");
    size_t fileModTime = GetFileModTime(scriptPath.c_str());
    
    while (!WindowShouldClose() || g_headLessMode) {
        //Hot reload
        if (GetFileModTime(scriptPath.c_str()) != fileModTime) {
            // std::cout << (js_dofile(runtime, scriptPath.c_str()) ? "True" : "False") << std::endl;
            fileModTime = GetFileModTime(scriptPath.c_str());
        }

        js_dostring(runtime, "onFrame();");
#ifdef multiplayer
        // Networking update
        std::string onMsgCall = "onMessage(\"";
        bool gotMsg = false;
        switch (g_eNetMode) {
            case NETWORK_MODE_HOST:
                g_nServer.Listen();
                if (g_nServer.messageAvailable()) {
                    onMsgCall+=g_nServer.getMessage() + "\");";
                    gotMsg = true;
                    //std::clog << onMsgCall << std::endl;
                }
                break;
            case NETWORK_MODE_CLIENT:
                g_nClient.Poll();
                if (g_nClient.messageAvailable()) {
                    onMsgCall+=g_nClient.getMessage() + "\");";
                    gotMsg = true;
                    //std::clog << onMsgCall << std::endl;
                }
                break;
            default:
                break;
        }
        if (g_eNetMode != NETWORK_MODE_SINGLEPLAYER && gotMsg) {
            //std::cout << "Running onMessage(): " << onMsgCall << std::endl;
            js_dostring(runtime, onMsgCall.c_str());
        }
#endif

    }
#ifdef multiplayer
    switch (g_eNetMode) {
        case NETWORK_MODE_HOST:
            g_nServer.Cleanup();
            break;
        case NETWORK_MODE_CLIENT:
            g_nClient.Cleanup();
            break;
        default:
            break;
    }
    if (!g_headLessMode) CloseWindow();
#endif
    return 0;
}