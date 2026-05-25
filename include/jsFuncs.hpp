//
// Created by benji on 14.05.26.
//

#ifndef RAYLIBPLAYGROUND_JSFUNCS_HPP
#define RAYLIBPLAYGROUND_JSFUNCS_HPP

#include <raylib.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <multiPlayer.hpp>

#define js_addFunc(from, to) \
js_newcfunction(runtime, from, to, 0); \
js_setglobal(runtime, to)

inline bool g_headLessMode = false;

static std::vector<std::pair<std::ifstream, std::string>> openFiles;
static std::unordered_map<std::string, size_t> fileNameMap;

//Helper functions
Color js_toColor(js_State *J, const unsigned short argPos) {
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

std::unordered_map<std::string, KeyboardKey> keyMap = {
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

inline Font defaultFont;

//js functions
static void jsHeadLessMode(js_State *J) { g_headLessMode = true; }
static void jsPrint(js_State *J) { std::cout << js_tostring(J, 1) << std::endl; js_pop(J, 1); }
static void jsBeginDrawing(js_State *J) { BeginDrawing(); }
static void jsEndDrawing(js_State *J) { EndDrawing(); }
static void jsClearBackGround(js_State *J) { ClearBackground(BLACK); }
static void jsDrawCircle(js_State *J) {
    const int x = js_tointeger(J, 1);
    const int y = js_tointeger(J, 2);
    const auto rad = static_cast<float>(js_tointeger(J, 3));

    const Color col = js_toColor(J, 4);

    DrawCircle(x, y, rad, col);
    //std::cout << "Draw at " << x << " " << y << " " << r << std::endl;
    js_pop(J, 4);
}
static void jsSetTargetFPS(js_State *J) { SetTargetFPS(js_tointeger(J, 1)); js_pop(J, 1); }
static void jsDrawFPS(js_State *J) {DrawFPS(js_tointeger(J, 1), js_tointeger(J, 2)); js_pop(J, 2); }
static void jsSin(js_State *J) { js_pushnumber(J, sin(js_tonumber(J, 1))); }
static void jsCos(js_State *J) { js_pushnumber(J, cos(js_tonumber(J, 1))); }
static void jsGetFrameTime(js_State *J) {js_pushnumber(J, GetFrameTime()); }
static void jsGetScreenWidth(js_State *J) { js_pushnumber(J, GetScreenWidth()); }
static void jsGetScreenHeight(js_State *J) { js_pushnumber(J, GetScreenHeight()); }
static void jsDrawRectangle(js_State *J) {
    DrawRectangle(js_tointeger(J, 1), js_tointeger(J, 2),js_tointeger(J, 3), js_tointeger(J, 4), js_toColor(J, 5) );
    js_pop(J, 5);
}
static void jsSetWindowTitle(js_State *J) { SetWindowTitle(js_tostring(J, 1)); js_pop(J, 1); }
static void jsGetCharPressed(js_State *J) {
    char buff[2] = {0, 0};
    buff[0] = GetCharPressed();
    if (buff[0]) {
        js_pushstring(J, buff);
    }
}

static void jsIsKeyDown(js_State *J) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyDown(keyMap[keyName]));
}
static void jsIsKeyPressed(js_State *J) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyPressed(keyMap[keyName]));
}
static void jsGetMouseX(js_State *J) { js_pushnumber(J, GetMouseX()); }
static void jsGetMouseY(js_State *J) { js_pushnumber(J, GetMouseY()); }

// DrawText("text", x, y, fontsize, COLOR);
static void jsDrawText(js_State *J) {
    DrawTextEx(defaultFont, js_tostring(J, 1), {static_cast<float>(js_tonumber(J, 2)), static_cast<float>(js_tonumber(J, 3))}, js_tointeger(J, 4), 1, js_toColor(J, 5)); js_pop(J, 5);
}

//OpenFile(filename: str)
static void jsOpenFile(js_State *J) {
    // Get the file name
    const std::string fileName = js_tostring(J, 1);
    const auto alredy_existing_file = fileNameMap.find(fileName);

    //Check if it's valid, else stop opening the file
    if (alredy_existing_file != fileNameMap.end()) {
        //std::cout << "This file has been opened before with id: " << alredy_existing_file->second << std::endl;
        js_pushnumber(J, alredy_existing_file->second);
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
    js_pushnumber(J, newID);
}

static void jsGetLine(js_State *J) {
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

static void jsAtEOF(js_State *J) {
    const size_t id = js_tointeger(J, 1);
    //js_pop(J, 1);
    js_pushboolean(J, openFiles[id].first.eof());
}

static void jsRewind(js_State *J) {
    //Close and reopen

    const size_t id = js_tointeger(J, 1);
    openFiles[id].first.clear();
    openFiles[id].first.sync();
    openFiles[id].first.close();
    openFiles[id].first.open(openFiles[id].second);
}

static void jsGetFileModTime(js_State *J) {
    js_pushnumber(J, GetFileModTime(js_tostring(J, 1)));
}

static void jsSetFont(js_State *J) {
    defaultFont = LoadFont(js_tostring(J, 1));
}

static void jsMaximizeWindow(js_State *J) {
    MaximizeWindow();
}

//Networking
//host / connect
//send
//getmsgCount
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
 * - Add a onTick() function to the runtime, that get's called every physics/networkFrame
 */

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

static void jsGetMessage(js_State *J) {
    switch (g_eNetMode) {
        case NETWORK_MODE_HOST:
            js_pushstring(J, g_nServer.m_sMsg.c_str());
            break;
        case NETWORK_MODE_CLIENT:
            js_pushstring(J, g_nServer.m_sMsg.c_str());
            break;
        default:
            js_pushundefined(J);
            break;
    }
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
        default:break;
    }
}

//3D
static void jsBeginMode3D(js_State *J) {

}

static void jsEndMode3D(js_State *J) {

}

inline void setupRaylibFuncs(js_State *runtime) {
    js_addFunc(jsPrint, "print");
    js_addFunc(jsBeginDrawing, "BeginDrawing");
    js_addFunc(jsEndDrawing, "EndDrawing");
    js_addFunc(jsClearBackGround, "ClearBackground");
    js_addFunc(jsSetTargetFPS, "SetTargetFPS");
    js_addFunc(jsDrawFPS, "DrawFPS");


    js_addFunc(jsSin, "sin");
    js_addFunc(jsCos, "cos");

    js_addFunc(jsHeadLessMode, "Headless");
    js_addFunc(jsGetFrameTime, "GetFrameTime");
    js_addFunc(jsGetScreenWidth, "GetScreenWidth");
    js_addFunc(jsGetScreenHeight, "GetScreenHeight");
    js_addFunc(jsMaximizeWindow, "MaximizeWindow");
    js_addFunc(jsSetWindowTitle, "SetWindowTitle");

    js_addFunc(jsGetCharPressed, "GetCharPressed");
    js_addFunc(jsIsKeyDown, "IsKeyDown");
    js_addFunc(jsIsKeyPressed, "IsKeyPressed");
    js_addFunc(jsGetMouseX, "GetMouseX");
    js_addFunc(jsGetMouseY, "GetMouseY");

    js_addFunc(jsDrawCircle, "DrawCircle");
    js_addFunc(jsDrawRectangle, "DrawRectangle");
    js_addFunc(jsDrawText, "DrawText");



    //file reading
    js_addFunc(jsGetFileModTime, "GetFileModTime");
    js_addFunc(jsSetFont, "SetFont");
    js_addFunc(jsOpenFile, "OpenFile");
    js_addFunc(jsGetLine, "GetLine");
    js_addFunc(jsAtEOF, "AtEOF");
    js_addFunc(jsRewind, "Rewind");

    //networking & multiplayer
    js_addFunc(jsHost, "Host");
    js_addFunc(jsIsHosting, "IsHosting");
    js_addFunc(jsConnect, "Connect");
    js_addFunc(jsGetMessage, "GetMessage");
    js_addFunc(jsSendMessage, "SendMessage");
}

#endif //RAYLIBPLAYGROUND_JSFUNCS_HPP
