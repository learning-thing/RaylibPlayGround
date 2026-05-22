#include <iostream>
#include <mujs.h>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <raylib.h>
#include <unordered_map>
#include <jsFuncs.hpp>
#include <chrono>

using namespace std::chrono_literals;

// later create a template
void create_template() {
    std::ofstream script("script.js");
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
"        BeginDrawing();\n"
"        ClearBackground();\n"
"        //DrawFPS(10, 10);\n"
"        if (player.x >= GetScreenWidth()) { velocity.x *= -1; }\n"
"        if (player.y >= GetScreenHeight()) { velocity.y *= -1; }\n"
"        if (player.x <= 0) { velocity.x *= -1; }\n"
"        if (player.y <= 0) { velocity.y *= -1; }\n"
"        player.x += velocity.x * GetFrameTime();\n"
"        player.y += velocity.y * GetFrameTime();\n"
"        DrawCircle(player.x, player.y, 20, circleColor);\n"
"        EndDrawing();\n"
"    }\n";
}

int main(const int argc, char **argv) {
    js_State *runtime = js_newstate(0, 0, 0);

    setupRaylibFuncs(runtime);

    std::string scriptPath = "script.js";
    if (argc > 1) scriptPath = argv[1];

    if (!std::filesystem::exists(scriptPath)) create_template();
    js_dofile(runtime, scriptPath.c_str());

    //Function on init
    js_dostring(runtime, "onStart();");
    auto now = std::chrono::system_clock::now().time_since_epoch()+std::chrono::seconds(10);

    InitWindow(1080, 720, "Hello world");

    //Onready function (after window creation)
    js_dostring(runtime, "onReady();");
    size_t fileModTime = GetFileModTime(scriptPath.c_str());
    while (!WindowShouldClose()) {
        //Hot reload
        if (GetFileModTime(scriptPath.c_str()) != fileModTime) {
            js_dofile(runtime, scriptPath.c_str());
            fileModTime = GetFileModTime(scriptPath.c_str());
        }
        js_dostring(runtime, "onFrame();");
    }
    return 0;
}