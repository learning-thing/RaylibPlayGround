//
// Created by benji on 14.05.26.
//

#ifndef RAYLIBPLAYGROUND_JSFUNCS_HPP
#define RAYLIBPLAYGROUND_JSFUNCS_HPP

#include <cassert>
#include <raylib.h>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <map>
#ifdef multiplayer
#include <multiPlayer.hpp>
#endif
#include <bits/this_thread_sleep.h>
#include "jsShader.hpp"
#include "../vendor/r3d/include/r3d/r3d_draw.h"
#include "../vendor/r3d/include/r3d/r3d_lighting.h"


#ifndef noserial
#include "serial.hpp"
#endif

#define jsFunc(name) static void name(js_State *J)
#define js_addFunc(from) \
js_newcfunction(runtime, from, & #from [2], 0); \
js_setglobal(runtime, & #from [2])

/*
#define js_addFunc(from, to) \
js_newcfunction(runtime, from, to, 0); \
js_setglobal(runtime, to)
*/

inline bool g_headLessMode = false;
inline char **args;
inline unsigned int argCount;
inline std::vector<std::pair<std::ifstream, std::string>> openFiles;
inline std::unordered_map<std::string, size_t> fileNameMap;
inline bool serialwarned = false;

// graphics stuff
inline std::vector<RenderTexture> renderTextures;
inline R3D_Material defaultMaterial;
inline std::vector<R3D_Mesh> g_vR3DMeshes;
inline std::vector<R3D_Light> g_vR3DLights;
inline std::vector<R3D_Model> g_vModels;


//Helper functions
static Vector3 js_toVec3(js_State *J, const unsigned short argPos) {
    // Get the color from an object
    js_getproperty(J, argPos, "x");
    const auto x = static_cast<float>(js_tonumber(J, js_gettop(J)-1));
    js_pop(J, 1);

    js_getproperty(J, argPos, "y");
    const auto y = static_cast<float>(js_tonumber(J, js_gettop(J)-1));
    js_pop(J, 1);

    js_getproperty(J, argPos, "z");
    const auto z = static_cast<float>(js_tonumber(J, js_gettop(J)-1));
    js_pop(J, 1);

    return {x, y, z};
}

//Helper functions
static Vector2 js_toVec2(js_State *J, const unsigned short argPos) {
    // Get the color from an object
    js_getproperty(J, argPos, "x");
    const auto x = static_cast<float>(js_tonumber(J, js_gettop(J)-1));
    js_pop(J, 1);

    js_getproperty(J, argPos, "y");
    const auto y = static_cast<float>(js_tonumber(J, js_gettop(J)-1));
    js_pop(J, 1);

    return {x, y};
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
    const auto fov = static_cast<float>(js_tonumber(J, argPos+1));
    js_pop(J, 1);

    return {position, target, up, fov, CAMERA_PERSPECTIVE};
}

static Camera2D js_toCamera2D(js_State *J, const unsigned short argPos) {
    js_getproperty(J, argPos, "offset");
    const auto Offset = js_toVec2(J, js_gettop(J)-1);
    js_pop(J, 1);

    js_getproperty(J, argPos, "target");
    const auto [targX, targY] = js_toVec2(J,js_gettop(J)-1);

    js_getproperty(J, argPos, "rotation");
    const float rotation = static_cast<float>(js_tonumber(J, js_gettop(J)-1));

    js_getproperty(J, argPos, "zoom");
    const float zoom = static_cast<float>(js_tonumber(J,js_gettop(J)-1));
    js_pop(J, 1);
    return {Offset, targX, targY, rotation, zoom};
}

static std::unordered_map<std::string, KeyboardKey> keyMap = {
    {"w", KEY_W },
    {"a", KEY_A },
    {"s", KEY_S },
    {"d", KEY_D },

    {"up",   KEY_UP    },
    {"down", KEY_DOWN  },
    {"left", KEY_LEFT  },
    {"right",KEY_RIGHT },

    {"space", KEY_SPACE          },
    {"shift", KEY_LEFT_SHIFT     },
    {"shift_r", KEY_RIGHT_SHIFT  },
    {"enter", KEY_ENTER          },
    {"backspace", KEY_BACKSPACE  },
    {"ctrl", KEY_LEFT_CONTROL    },
    {"ctrl_r", KEY_RIGHT_CONTROL },

    {"e", KEY_E },
    {"q", KEY_Q },
    {"x", KEY_X },
    {"y",  KEY_Y  }
};
static Font defaultFont;

jsFunc(jsSave) {
    std::ofstream file(js_tostring(J, 1));
    file << js_tostring(J, 2);
}

//js functions
jsFunc(jsArgCount) {
    js_pushnumber(J, argCount);
}
jsFunc(jsArgs) {
    if (const auto pos = js_tointeger(J, 1); pos < argCount-1 && pos > 0)
        js_pushstring(J, args[pos]);
    else
        js_pushstring(J, args[argCount-1]);
}
jsFunc(jsHeadLessMode) { g_headLessMode = true; }
jsFunc(jsPrint) { std::cout << js_tostring(J, 1) << std::endl; js_pop(J, 1); }
jsFunc(jsBeginDrawing) { BeginDrawing(); }
jsFunc(jsEndDrawing) { EndDrawing(); }
jsFunc(jsClearBackground) { ClearBackground(js_toColor(J, 1)); }
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
jsFunc(jsAllowWindowResize) { SetWindowState(FLAG_WINDOW_RESIZABLE); }
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
    } else {
        js_pushundefined(J);
    }
}

jsFunc(jsMeasureTextW) {
    const char *text = js_tostring(J, 1);
    const auto fontSize = static_cast<float>(js_tonumber(J, 2));
    const float spacing = 1;
    const auto m = MeasureTextEx(defaultFont, text, fontSize, spacing).x;
    //std::clog << text << " has width: " << m << "\n";
    js_pushnumber(J, m);
}

jsFunc(jsMeasureTextH) {
    js_pushnumber(J, MeasureTextEx(defaultFont, js_tostring(J, 1), js_tonumber(J, 2), 1).y);
}

jsFunc(jsIsKeyDown) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyDown(keyMap[keyName]));
}
jsFunc (jsIsKeyPressed) {
    const std::string keyName = js_tostring(J, 1); js_pushboolean(J, IsKeyPressed(keyMap[keyName]));
}
jsFunc(jsGetMouseX) { js_pushnumber(J, GetMouseX()); }
jsFunc(jsGetMouseY) { js_pushnumber(J, GetMouseY()); }
jsFunc(jsGetMouseDeltaX) { js_pushnumber(J, GetMouseDelta().x); }
jsFunc(jsGetMouseDeltaY) { js_pushnumber(J, GetMouseDelta().y); }
jsFunc(jsIsMouseButtonDown) { js_pushboolean(J, IsMouseButtonDown(static_cast<int>(js_tonumber(J, 1)))); }
jsFunc(jsIsMouseButtonPressed) { js_pushboolean(J, IsMouseButtonPressed(static_cast<int>(js_tonumber(J, 1)))); }
jsFunc(jsGetMouseWheelMove) { js_pushnumber(J, GetMouseWheelMove()); }

jsFunc(jsDrawLine) {
    DrawLine(static_cast<int>(js_tonumber(J, 1)), static_cast<int>(js_tonumber(J, 2)), static_cast<int>(js_tonumber(J, 3)), static_cast<int>(js_tonumber(J, 4)), js_toColor(J, 5));
}

// DrawText("text", x, y, fontsize, COLOR);
jsFunc(jsDrawText) {
    DrawTextEx(defaultFont, js_tostring(J, 1), {static_cast<float>(js_tonumber(J, 2)), static_cast<float>(js_tonumber(J, 3))}, static_cast<float>(js_tonumber(J, 4)), 1, js_toColor(J, 5));
    js_pop(J, 5);
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

jsFunc(jsCloseFile) {
    openFiles[js_tointeger(J, 1)].first.close();
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

static std::vector<jsShader> shaders;

#ifdef multiplayer
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
    R3D_Begin(cam);
}

jsFunc(jsEndMode3D) {
    R3D_End();
}

jsFunc(jsDrawGrid) {
    const int a = js_tointeger(J, 1);
    const int b = js_tointeger(J, 2);
    DrawGrid(a, b);

    js_pop(J, 2);
}

jsFunc(jsLoadShader) {
    const auto s = static_cast<size_t>(shaders.size());
    shaders.emplace_back(js_tostring(J, 1), js_tostring(J, 2));
    js_pushnumber(J, s);
}

jsFunc(jsBeginShader) {
    shaders[js_tointeger(J, 1)].begin();
}

jsFunc(jsEndShader) {
    EndShaderMode();
}

jsFunc(jsGetShaderLoc) {
    const char *name = js_tostring(J, 2);
    const int loc = shaders[js_tointeger(J, 1)].getUniformLoc(name);
    std::cout << "Shader location of " << name << " is " << loc << std::endl;

    js_pushnumber(J, loc);
}

jsFunc(jsSetUniform) {
    //js_typeof()
    switch (js_typeof(J, 3)[0]) {
        case 'n': {
            const jsShader &shader = shaders[js_tointeger(J, 1)];
            const int location = js_tointeger(J, 2);
            const auto value = static_cast<float>(js_tonumber(J, 3));
            shader.setUniform(location, value);
            //std::cout << "uniform " << location << " of shader " << js_tointeger(J, 1) << " set to " << value << std::endl;
            break;
        }
        default:
            std::cerr << "Unhandled Uniform type: " << js_typeof(J, 3) << std::endl;
            break;
    }
}

inline std::map<std::string, Texture> images;

jsFunc(jsDrawImage) {
    const std::string imgPath = js_tostring(J, 1);
    const auto x      = static_cast<float>(js_tonumber(J, 2));
    const auto y      = static_cast<float>(js_tonumber(J, 3));
    const auto width  = static_cast<float>(js_tonumber(J, 4));
    const auto height = static_cast<float>(js_tonumber(J, 5));

    //If that image has already been loaded
    if (images.contains(imgPath)) {
        //images[imgPath], {0, 0, width, height}, {x, y}, WHITE
        if (width == 0.0f) {
            DrawTexturePro(images[imgPath], {0, 0, static_cast<float>(images[imgPath].width), static_cast<float>(images[imgPath].width)}, {x, y, static_cast<float>(images[imgPath].width), static_cast<float>(images[imgPath].height)}, {0, 0}, 0, WHITE);
        } else {
            DrawTexturePro(images[imgPath], {0, 0, static_cast<float>(images[imgPath].width), static_cast<float>(images[imgPath].width)}, {x, y, width, height}, {0, 0}, 0, WHITE);
        }
    } else {
        //Load the image
        const Image img = LoadImage(imgPath.c_str());
        images.emplace(imgPath, LoadTextureFromImage(img));
        UnloadImage(img);
    }
}

jsFunc(jsDrawCube) {
    // Get the color from an object
    constexpr unsigned int argPos = 1;

    assert(js_hasproperty(J, argPos, "x"));
    js_getproperty(J, argPos, "x");
    const auto x = static_cast<float>(js_tonumber(J, js_gettop(J)-1));

    assert(js_hasproperty(J, argPos, "y"));
    js_getproperty(J, argPos, "y");
    const auto y = static_cast<float>(js_tonumber(J, js_gettop(J)-1));

    assert(js_hasproperty(J, argPos, "z"));
    js_getproperty(J, argPos, "z");
    const auto z = static_cast<float>(js_tonumber(J, js_gettop(J)-1));

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
    //DrawCube({x, y, z}, width, height, length, col);
    R3D_DrawMesh(g_vR3DMeshes[0], defaultMaterial, {x, y, z}, 10);
}

jsFunc(jsGenMeshCube) {
    const auto s = g_vR3DMeshes.size();
    g_vR3DMeshes.emplace_back(js_tonumber(J, 1), js_tonumber(J, 2), js_tonumber(J, 3));
    js_pushnumber(J, static_cast<double>(s));
}

jsFunc(jsDrawMesh) {
    const auto mesh = static_cast<size_t>(js_tonumber(J, 1));
    R3D_DrawMesh(g_vR3DMeshes[mesh], defaultMaterial, {0, 0, 0}, 10);
}

jsFunc(jsDrawCubeWires) {
    // Get the color from an object
    constexpr unsigned int argPos = 1;
    const auto j_argCount = js_gettop(J);

    assert(js_hasproperty(J, argPos, "x"));
    js_getproperty(J, argPos, "x");
    const auto x      = static_cast<float>(js_tonumber(J, j_argCount + 2));

    js_getproperty(J, argPos, "y");
    const auto y      = static_cast<float>(js_tonumber(J, j_argCount + 3));

    js_getproperty(J, argPos, "z");
    const auto z      = static_cast<float>(js_tonumber(J, j_argCount + 4));

    const auto width  = static_cast<float>(js_tonumber(J, 2));
    const auto height = static_cast<float>(js_tonumber(J, 3));
    const auto length = static_cast<float>(js_tonumber(J, 4));

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
    const auto id = g_vModels.size();
    g_vModels.push_back(R3D_LoadModel(js_tostring(J, 1)));
    js_pushnumber(J, static_cast<double>(id));
}

jsFunc(jsDrawModel) {
    const auto modelID = static_cast<size_t>(js_tointeger(J, 1));
    R3D_DrawModel(g_vModels[modelID], js_toVec3(J, 2), static_cast<float>(js_tonumber(J, 3)));
}

jsFunc(jsCloseWindow) {
    CloseWindow();
}

jsFunc(jsResizeWindow) {
    SetWindowSize(static_cast<int>(js_tonumber(J, 1)), static_cast<int>(js_tonumber(J, 2)));
}

#ifndef noserial
inline Serial* serial = nullptr;
#endif

//Serial stuff
jsFunc(jsOpenSerial) {
#ifndef noserial
    const std::string a = js_tostring(J, 1);
    const int baudrate = js_tointeger(J, 2);
    try {
        serial = new Serial(a, baudrate);
    } catch (const boost::wrapexcept<boost::system::system_error>& e) {
        std::cerr << "Failed to open serial: \n\t" << e.what() << std::endl;
        delete serial;
    }
#endif
}

jsFunc(jsReadSerial) {
#ifndef noserial
    if (serial) {
        const std::string line = serial->readLineAsync();
        if (!line.empty()) {
            js_pushstring(J, line.c_str());
        } else {
            js_pushundefined(J);
        }
    } else {
        js_pushundefined(J);
        if (serialwarned) return;
        std::cerr << "Attempted to use serial when none was opened" << std::endl;
        serialwarned = true;
    }
#else
    js_pushundefined(J);
#endif
}

jsFunc(jsWriteSerial) {
#ifndef noserial
    if (!serial) {
        if (serialwarned) return;
        std::cerr << "Attempted to use serial when none was opened" << std::endl;
        serialwarned = true;
        return;
    }
    if (js_typeof(J, 1)[0] == 'n')
        serial->write(js_tointeger(J, 1));
    else
        serial->write(js_tostring(J, 1));
#endif
}

jsFunc(jsIsSerialOpen) {
#ifndef noserial
    if (!serial) {
        js_pushboolean(J, false); return;
        if (serialwarned) return;
        std::cerr << "Attempted to use serial when none was opened" << std::endl;
        serialwarned = true;
    }
    js_pushboolean(J, serial->isOpen());
#else
    js_pushundefined(J);
#endif
}

jsFunc(jsSerialRetry) {
#ifndef noserial
    if (!serial) {std::cerr << "Serial was never opened!\n"; return; }
    serial->retry();
#endif
}

jsFunc(jsCreateRenderTexture) {
    const auto id = renderTextures.size();
    const auto width=js_tointeger(J, 1), height=js_tointeger(J, 2);
    renderTextures.push_back(LoadRenderTexture(width, height));
    js_pushnumber(J, id);
}

jsFunc(jsBeginTextureMode) {
    const auto texId = js_tointeger(J, 1);
    const auto& text = renderTextures[texId];
    BeginTextureMode(text);
}

jsFunc(jsEndTextureMode) {
    EndTextureMode();
}

jsFunc(jsTexFilterPoint) {
    const auto& texture = renderTextures[js_tointeger(J, 1)];
    SetTextureFilter(texture.texture, TEXTURE_FILTER_POINT);
}

jsFunc(jsBeginMode2D) {
    const Camera2D cam = js_toCamera2D(J, 1);
    BeginMode2D(cam);
}

jsFunc(jsEndMode2D) {
    EndMode2D();
}

jsFunc(jsDrawRenderTexture) {
    const auto& texture = renderTextures[js_tointeger(J, 1)];
    DrawTexture(texture.texture, 0, 0, WHITE);
}

inline void setupRaylibFuncs(js_State *runtime) {
    js_addFunc(jsPrint);
    js_addFunc(jsBeginDrawing);
    js_addFunc(jsEndDrawing);
    js_addFunc(jsBeginMode3D);
    js_addFunc(jsEndMode3D);
    js_addFunc(jsClearBackground);
    js_addFunc(jsSetTargetFPS);
    js_addFunc(jsSleep);
    js_addFunc(jsDrawFPS);

    //Math'n'shit
    js_addFunc(jsSin);
    js_addFunc(jsCos);

    //text and strings
    js_addFunc(jsMeasureTextW);
    js_addFunc(jsMeasureTextH);

    //window n shit
    js_addFunc(jsHeadLessMode);
    js_addFunc(jsGetFrameTime);
    js_addFunc(jsGetScreenWidth);
    js_addFunc(jsGetScreenHeight);
    js_addFunc(jsMaximizeWindow);
    js_addFunc(jsSetWindowTitle);
    js_addFunc(jsCloseWindow);
    js_addFunc(jsAllowWindowResize);

    //inputn allat
    js_addFunc(jsGetCharPressed);
    js_addFunc(jsIsKeyDown);
    js_addFunc(jsIsKeyPressed);
    js_addFunc(jsGetMouseX);
    js_addFunc(jsGetMouseY);
    js_addFunc(jsGetMouseDeltaX);
    js_addFunc(jsGetMouseDeltaY);
    js_addFunc(jsIsMouseButtonDown);
    js_addFunc(jsIsMouseButtonPressed);
    js_addFunc(jsGetMouseWheelMove);

    //Draw stuff
    js_addFunc(jsDrawCircle);
    js_addFunc(jsDrawRectangle);
    js_addFunc(jsDrawRectangleLines);
    js_addFunc(jsDrawText);
    js_addFunc(jsDrawImage);
    js_addFunc(jsDrawLine);

    // - 3D
    js_addFunc(jsDrawGrid);
    js_addFunc(jsDrawCube);
    js_addFunc(jsDrawCubeV);
    js_addFunc(jsDrawCubeWires);
    js_addFunc(jsLoadModel);
    js_addFunc(jsGenMeshCube);

    // r3d
    js_addFunc(jsDrawMesh);
    js_addFunc(jsDrawModel);

    // - Shader
    js_addFunc(jsLoadShader);
    js_addFunc(jsGetShaderLoc);
    js_addFunc(jsSetUniform);
    js_addFunc(jsBeginShader);
    js_addFunc(jsEndShader);

    // file handling
    js_addFunc(jsGetFileModTime);
    js_addFunc(jsSetFont);
    js_addFunc(jsOpenFile);
    js_addFunc(jsGetLine);
    js_addFunc(jsAtEOF);
    js_addFunc(jsRewind);
    js_addFunc(jsSave);
    js_addFunc(jsCloseFile);

#ifndef noserial
    // Serial (wtf am I doing)
    js_addFunc(jsOpenSerial);
    js_addFunc(jsWriteSerial);
    js_addFunc(jsIsSerialOpen);
    js_addFunc(jsSerialRetry);
    js_addFunc(jsReadSerial);
#endif

    js_addFunc(jsArgCount);
    js_addFunc(jsArgs);
    // networking & multiplayer
#ifdef multiplayer
    js_addFunc(jsHost);
    js_addFunc(jsIsHosting);
    js_addFunc(jsConnect);
    js_addFunc(jsSendMessage);
#endif

    js_addFunc(jsCreateRenderTexture);
    js_addFunc(jsBeginTextureMode);
    js_addFunc(jsEndTextureMode);
    js_addFunc(jsTexFilterPoint);
    js_addFunc(jsDrawRenderTexture);

    js_addFunc(jsBeginMode2D);
    js_addFunc(jsEndMode2D);
}

static volatile const char *inBinaryMsg = "Bro please don't reverse engineer, there's source code on github";
#endif //RAYLIBPLAYGROUND_JSFUNCS_HPP
