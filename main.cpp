#include <iostream>
#include <mujs.h>
#include <filesystem>
#include <fstream>
#include <raylib.h>
#include <unordered_map>
#include <jsFuncs.hpp>
#include <chrono>
#include <multiPlayer.hpp>

using namespace std::chrono_literals;

// later create a template
void create_template(const char *projectName) {
    std::ofstream script(projectName);
    script <<
"var player;\n var velocity;\n"
"var circleColor = {\n"
"   r: 50,\n"
"   g: 50,\n"
"   b: 255,\n"
"   a: 255\n"
"};\n\n"
"velocity = { x: 200, y: 200 }\n\n"
"function onStart() {\n"
"   x = 60;\n"
"   y = 60;\n\n"
"   print(\"Starting\");\n"
"   SetTargetFPS(60);\n"
"   \n"
"   player   = { x: 0, y: 0 }\n"
"}\n\n"
"function onReady() {}\n"
"function onFrame() {\n"
"    BeginDrawing();\n"
"    ClearBackground();\n"
"    //DrawFPS(10, 10);\n"
"    if (player.x >= GetScreenWidth()) { velocity.x *= -1; }\n"
"    if (player.y >= GetScreenHeight()) { velocity.y *= -1; }\n"
"    if (player.x <= 0) { velocity.x *= -1; }\n"
"    if (player.y <= 0) { velocity.y *= -1; }\n"
"    player.x += velocity.x * GetFrameTime();\n"
"    player.y += velocity.y * GetFrameTime();\n"
"    DrawCircle(player.x, player.y, 20, circleColor);\n"
"    EndDrawing();\n"
"}\n";
}

int main(const int argc, char **argv) {
    js_State *runtime = js_newstate(nullptr, nullptr, 0);
    setupRaylibFuncs(runtime);

    std::string scriptPath = "script.js";
    if (argc > 1) scriptPath = argv[1];

    if (!std::filesystem::exists(scriptPath)) create_template(scriptPath.c_str());
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
            js_dofile(runtime, scriptPath.c_str());
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