//
// Created by benji on 14.05.26.
//

#ifndef RAYLIBPLAYGROUND_JSFUNCS_HPP
#define RAYLIBPLAYGROUND_JSFUNCS_HPP

#include <assert.h>
#include <raylib.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <cstring>
#ifdef multiplayer
#include <multiPlayer.hpp>
#endif
#include <bits/this_thread_sleep.h>

#define jsFunc(name) static void name(js_State *J)

#define js_addFunc(from, to) \
js_newcfunction(runtime, from, to, 0); \
js_setglobal(runtime, to)

inline bool g_headLessMode = false;

static std::vector<std::pair<std::ifstream, std::string>> openFiles;
static std::unordered_map<std::string, size_t> fileNameMap;

//Helper functions
static Vector3 js_toVec3(js_State *J, const unsigned short argPos) {
    // Get the color from an object
    js_getproperty(J, argPos, "x");
    const auto x = static_cast<float>(js_tonumber(J, argPos + 1));
    js_pop(J, 1);

    js_getproperty(J, argPos, "y");
    const auto y = static_cast<float>(js_tonumber(J, argPos + 1));
    js_pop(J, 1);

    js_getproperty(J, argPos, "z");
    const auto z = static_cast<float>(js_tonumber(J, argPos + 1));
    js_pop(J, 1);

    return {x, y, z};
}

//Helper functions
static Color js_toColor(js_State *J, const unsigned short argPos) {
    // Get the color from an object
    js_getproperty(J, argPos, "r");
    const unsigned char r = js_tointeger(J, argPos + 1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "g");
    const unsigned char g = js_tointeger(J, argPos + 1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "b");
    const unsigned char b = js_tointeger(J, argPos + 1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "a");
    const unsigned char a = js_tointeger(J, argPos + 1);
    js_pop(J, 1);

    return {r, g, b, a};
}

static Camera3D js_toCamera(js_State *J, const unsigned short argPos) {
    js_getproperty(J, argPos, "position");
    const Vector3 position = js_toVec3(J, argPos+1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "target");
    const Vector3 target = js_toVec3(J, argPos+1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "up");
    const Vector3 up = js_toVec3(J, argPos+1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "fovy");
    const float fov = static_cast<float>(js_tonumber(J, argPos+1));
    js_pop(J, 1);

    return {position, target, up, fov, CAMERA_PERSPECTIVE};
}

static std::unordered_map<std::string, KeyboardKey> keyMap = {
    {"w", KEY_W },
    {"a", KEY_A },
    {"s", KEY_S },
    {"d", KEY_D },

    {"up", KEY_UP},
    {"down", KEY_DOWN   },
    {"left", KEY_LEFT   },
    {"right", KEY_RIGHT },

    {"space", KEY_SPACE},
    {"enter", KEY_ENTER},

    {"e", KEY_E },
    {"q", KEY_Q },
    {"x", KEY_X },
    {"y",  KEY_Y  }
};
static Font defaultFont;

//js functions
jsFunc(jsHeadLessMode) { g_headLessMode = true; }
jsFunc(jsPrint) { std::cout << js_tostring(J, 1) << std::endl; js_pop(J, 1); }
jsFunc(jsBeginDrawing) { BeginDrawing(); }
jsFunc(jsEndDrawing) { EndDrawing(); }
jsFunc(jsClearBackGround) { ClearBackground(js_toColor(J, 1)); }
jsFunc(jsDrawCircle) {
    const int x = js_tointeger(J, 1);
    const int y = js_tointeger(J, 2);
    const auto rad = static_cast<float>(js_tointeger(J, 3));

    const Color col = js_toColor(J, 4);

    DrawCircle(x, y, rad, col);
    //std::cout << "Draw at " << x << " " << y << " " << r << std::endl;
    js_pop(J, 4);
}
jsFunc(jsSetTargetFPS) { SetTargetFPS(js_tointeger(J, 1)); js_pop(J, 1); }
jsFunc(jsDrawFPS) {DrawFPS(js_tointeger(J, 1), js_tointeger(J, 2)); js_pop(J, 2); }
jsFunc(jsSin) { js_pushnumber(J, sin(js_tonumber(J, 1))); }
jsFunc(jsCos) { js_pushnumber(J, cos(js_tonumber(J, 1))); }
jsFunc(jsGetFrameTime) {js_pushnumber(J, GetFrameTime()); }
jsFunc(jsGetScreenWidth) { js_pushnumber(J, GetScreenWidth()); }
jsFunc(jsGetScreenHeight) { js_pushnumber(J, GetScreenHeight()); }
jsFunc(jsDrawRectangle) {
    DrawRectangle(js_tointeger(J, 1), js_tointeger(J, 2),js_tointeger(J, 3), js_tointeger(J, 4), js_toColor(J, 5) );
    js_pop(J, 5);
}
jsFunc(jsDrawRectangleLines) {
    DrawRectangleLines(js_tointeger(J, 1), js_tointeger(J, 2), js_tointeger(J, 3), js_tointeger(J, 4),
                       js_toColor(J, 5));
    js_pop(J, 5);
}
jsFunc(jsSetWindowTitle) { SetWindowTitle(js_tostring(J, 1)); js_pop(J, 1); }
jsFunc(jsGetCharPressed) {
    char buff[2] = {0, 0};
    buff[0] = static_cast<char>(GetCharPressed());
    if (buff[0]) {
        js_pushstring(J, buff);
    }
}

jsFunc(jsIsKeyDown) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyDown(keyMap[keyName]));
}
jsFunc (jsIsKeyPressed) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyPressed(keyMap[keyName]));
}
jsFunc(jsGetMouseX) { js_pushnumber(J, GetMouseX()); }
jsFunc(jsGetMouseY) { js_pushnumber(J, GetMouseY()); }
jsFunc(jsGetMouseDeltaX) {
    js_pushnumber(J, GetMouseDelta().x);
}
jsFunc(jsGetMouseDeltaY) {
    js_pushnumber(J, GetMouseDelta().y);
}

    // DrawText("text", x, y, fontsize, COLOR);
jsFunc(jsDrawText) {
    DrawTextEx(defaultFont, js_tostring(J, 1), {static_cast<float>(js_tonumber(J, 2)), static_cast<float>(js_tonumber(J, 3))}, static_cast<float>(js_tonumber(J, 4)), 1, js_toColor(J, 5)); js_pop(J, 5);
}

//OpenFile(filename: str)
jsFunc(jsOpenFile) {
    // Get the file name
    const std::string fileName = js_tostring(J, 1);

    //Check if it's valid, else stop opening the file
    if (const auto already_existing_file = fileNameMap.find(fileName); already_existing_file != fileNameMap.end()) {
        //std::cout << "This file has been opened before with id: " << already_existing_file->second << std::endl;
        js_pushnumber(J, static_cast<double>(already_existing_file->second));
        return;
    }

    // Get a file id
    const size_t newID = openFiles.size();
    fileNameMap[fileName] = newID;
    openFiles.emplace_back(fileName, fileName);
    if (!openFiles[newID].first.is_open()) {
        std::cerr << "Failed to open " << fileName << std::endl;
        js_pushundefined(J);
    }

    js_pop(J, 1);
    js_pushnumber(J, static_cast<double>(newID));
}

jsFunc(jsGetLine) {
    //get by id
    const size_t id = js_tointeger(J, 1);
    const int maxChars = js_tointeger(J, 2);
    auto *buffer = new char[maxChars];
    std::memset(buffer, 0, maxChars);//set to 0

    openFiles[id].first.getline(buffer, maxChars, '\n');

    //max characters
    js_pushstring(J, buffer);
    delete[] buffer;
}

jsFunc(jsAtEOF) {
    const size_t id = js_tointeger(J, 1);
    //js_pop(J, 1);
    js_pushboolean(J, openFiles[id].first.eof());
}

jsFunc(jsRewind) {
    //Close and reopen
    const size_t id = js_tointeger(J, 1);
    openFiles[id].first.clear();
    openFiles[id].first.sync();
    openFiles[id].first.close();
    openFiles[id].first.open(openFiles[id].second);
}

jsFunc(jsGetFileModTime) {
    js_pushnumber(J, static_cast<double>(GetFileModTime(js_tostring(J, 1))));
}

static void jsSetFont(js_State *J) {
    defaultFont = LoadFont(js_tostring(J, 1));
}

jsFunc(jsMaximizeWindow) {
    MaximizeWindow();
}

//Networking
//host / connect
//send
//get message count
//GetMessage
/*
 * For multiplayer later:
 * - jsonServerStart() {}
 * - jsOnPlayerJoined() {}
 * - bool isHosting()
 * - get_client_id()
 * - host_server(PORT)
 * - connect_server(address, port)
 * - sync("varName");//add to list of vars to synchronize at the end of frame
 * // Should probably:
 * - Add a onTick() function to the runtime, that gets called every physics/networkFrame
 */

static std::vector<Shader> shaders;

#ifdef multilayer
inline networkingMode_t g_eNetMode = NETWORK_MODE_SINGLEPLAYER;
inline Server g_nServer;
inline Client g_nClient;

static void jsHost(js_State *J) {
    g_eNetMode = NETWORK_MODE_HOST;
    g_nServer.Initialize(js_tointeger(J, 1));
}
static void jsIsHosting(js_State *J) {
    js_pushboolean(J, (g_eNetMode == NETWORK_MODE_HOST));
}
static void jsConnect(js_State *J) {
    std::clog << "Attempting to connect to: " << js_tostring(J, 1) << std::endl;
    g_eNetMode = NETWORK_MODE_CLIENT;
    g_nClient.Connect(js_tostring(J, 1));
}

static void jsSendMessage(js_State *J) {
    // Server sends to all clients by default?
    switch (g_eNetMode) {
        case NETWORK_MODE_CLIENT:
            g_nClient.Send(js_tostring(J, 1));
            break;
        case NETWORK_MODE_HOST:
            g_nServer.SendStringAllClients(js_tostring(J, 1));
            break;
        default: break;
    }
}
#endif

jsFunc(jsSleep) {
    std::this_thread::sleep_for(std::chrono::milliseconds( js_tointeger(J, 1)) );
}

//3D
jsFunc(jsBeginMode3D) {
    const Camera3D cam = js_toCamera(J, 1);
    BeginMode3D(cam);
}

inline void jsEndMode3D(js_State *J) {
    EndMode3D();
}

jsFunc(jsDrawGrid) {
    const int a = js_tointeger(J, 1);
    const int b = js_tointeger(J, 2);
    DrawGrid(a, b);
    js_pop(J, 2);
}

jsFunc(jsLoadShader) {
    shaders.push_back(LoadShader(js_tostring(J, 1), js_tostring(J, 2)));
    js_pushnumber(J, static_cast<double>(shaders.size()));
}

jsFunc(jsBeginShader) {
    BeginShaderMode(shaders[js_tointeger(J, 1)]);
}

jsFunc(jsEndShader) {
    EndShaderMode();
}

jsFunc(jsSetUniform) {

}

jsFunc(jsDrawCube) {
    // Get the color from an object
    constexpr unsigned int argPos = 1;
    const unsigned int argCount = 5;

    //TODO: Use js_gettop to get stuff
    assert(js_hasproperty(J, argPos, "x"));
    js_getproperty(J, argPos, "x");
    const float x = js_tonumber(J, js_gettop(J)-1);

    assert(js_hasproperty(J, argPos, "y"));
    js_getproperty(J, argPos, "y");
    const float y = js_tonumber(J, js_gettop(J)-1);

    assert(js_hasproperty(J, argPos, "z"));
    js_getproperty(J, argPos, "z");
    const float z = js_tonumber(J, js_gettop(J)-1);

    const float width = js_tonumber(J, 2);
    const float height = js_tonumber(J, 3);
    const float length = js_tonumber(J, 4);

    Color col;
    //assert(js_hasproperty(J, 5, "r"));
    js_getproperty(J, 5, "r");
    col.r = js_tointeger(J, js_gettop(J)-1);

    //assert(js_hasproperty(J, 5, "g"));
    js_getproperty(J, 5, "g");
    col.g = js_tointeger(J, js_gettop(J)-1);

    //assert(js_hasproperty(J, 5, "b"));
    js_getproperty(J, 5, "b");
    col.b = js_tointeger(J, js_gettop(J)-1);

    //assert(js_hasproperty(J, 5, "a"));
    js_getproperty(J, 5, "a");
    col.a = js_tointeger(J, js_gettop(J)-1);

    //std::cout << "Drawing cube" << std::endl;
    //std::cout << x << y << z << width << height << length;
    DrawCube({x, y, z}, width, height, length, col);
}

jsFunc(jsDrawCubeWires) {
    // Get the color from an object
    const unsigned int argPos = 1;
    const unsigned int argCount = 5;

    assert(js_hasproperty(J, argPos, "x"));
    js_getproperty(J, argPos, "x");
    const float x = js_tonumber(J, argCount + 2);

    js_getproperty(J, argPos, "y");
    const float y = js_tonumber(J, argCount + 3);

    js_getproperty(J, argPos, "z");
    const float z = js_tonumber(J, argCount + 4);

    const float width = js_tonumber(J, 2);
    const float height = js_tonumber(J, 3);
    const float length = js_tonumber(J, 4);

    Color col;
    assert(js_hasproperty(J, 5, "r"));
    js_getproperty(J, 5, "r");
    col.r = js_tointeger(J, js_gettop(J)-1);

    assert(js_hasproperty(J, 5, "g"));
    js_getproperty(J, 5, "g");
    col.g = js_tointeger(J, js_gettop(J)-1);

    assert(js_hasproperty(J, 5, "b"));
    js_getproperty(J, 5, "b");
    col.b = js_tointeger(J, js_gettop(J)-1);

    assert(js_hasproperty(J, 5, "a"));
    js_getproperty(J, 5, "a");
    col.a = js_tointeger(J, js_gettop(J)-1);

    DrawCubeWires({x, y, z}, width, height, length, col);
}

//TODO: Fix this shit
//- toVec function needs to use getTop
//- currently toVec and toColor are still broken if used as non-first argument
jsFunc(jsDrawCubeV) {
    DrawCubeV(js_toVec3(J, 1), js_toVec3(J, 2), js_toColor(J, 3));
}

//TODO: Load models into vec or something idk
jsFunc(jsLoadModel) {

}

jsFunc(jsCloseWindow) {
    CloseWindow();
}

inline void setupRaylibFuncs(js_State *runtime) {
    js_addFunc(jsPrint, "print");
    js_addFunc(jsBeginDrawing, "BeginDrawing");
    js_addFunc(jsEndDrawing, "EndDrawing");
    js_addFunc(jsBeginMode3D, "BeginMode3D");
    js_addFunc(jsEndMode3D, "EndMode3D");
    js_addFunc(jsClearBackGround, "ClearBackground");
    js_addFunc(jsSetTargetFPS, "SetTargetFPS");
    js_addFunc(jsSleep, "Sleep");
    js_addFunc(jsDrawFPS, "DrawFPS");

    //Math'n'shit
    js_addFunc(jsSin, "sin");
    js_addFunc(jsCos, "cos");

    //window n shit
    js_addFunc(jsHeadLessMode, "Headless");
    js_addFunc(jsGetFrameTime, "GetFrameTime");
    js_addFunc(jsGetScreenWidth, "GetScreenWidth");
    js_addFunc(jsGetScreenHeight, "GetScreenHeight");
    js_addFunc(jsMaximizeWindow, "MaximizeWindow");
    js_addFunc(jsSetWindowTitle, "SetWindowTitle");
    js_addFunc(jsCloseWindow, "CloseWindow");

    js_addFunc(jsGetCharPressed, "GetCharPressed");
    js_addFunc(jsIsKeyDown, "IsKeyDown");
    js_addFunc(jsIsKeyPressed, "IsKeyPressed");
    js_addFunc(jsGetMouseX, "GetMouseX");
    js_addFunc(jsGetMouseY, "GetMouseY");
    js_addFunc(jsGetMouseDeltaX, "GetMouseDeltaX");
    js_addFunc(jsGetMouseDeltaY, "GetMouseDeltaY");

    //Draw stuff
    js_addFunc(jsDrawCircle, "DrawCircle");
    js_addFunc(jsDrawRectangle, "DrawRectangle");
    js_addFunc(jsDrawRectangleLines, "DrawRectangleLines");
    js_addFunc(jsDrawText, "DrawText");
    // - 3D
    js_addFunc(jsDrawGrid, "DrawGrid");
    js_addFunc(jsDrawCube, "DrawCube");
    js_addFunc(jsDrawCubeV, "DrawCubeV");
    js_addFunc(jsDrawCubeWires, "DrawCubeWires");
    // - Shader
    js_addFunc(jsLoadShader, "LoadShader");
    js_addFunc(jsBeginShader, "BeginShader");
    js_addFunc(jsEndShader, "EndShader");

    //file reading
    js_addFunc(jsGetFileModTime, "GetFileModTime");
    js_addFunc(jsSetFont, "SetFont");
    js_addFunc(jsOpenFile, "OpenFile");
    js_addFunc(jsGetLine, "GetLine");
    js_addFunc(jsAtEOF, "AtEOF");
    js_addFunc(jsRewind, "Rewind");

    //networking & multiplayer
#ifdef multiplayer
    js_addFunc(jsHost, "Host");
    js_addFunc(jsIsHosting, "IsHosting");
    js_addFunc(jsConnect, "Connect");
    js_addFunc(jsSendMessage, "SendMessage");
#endif

}

static volatile const char *inBinaryMsg = "Bro please don't reverse engineer, there's source code on github";
#endif //RAYLIBPLAYGROUND_JSFUNCS_HPP
