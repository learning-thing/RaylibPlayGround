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
#include "vendor/r3d/include/r3d/r3d.h"

using namespace std::chrono_literals;

int main(const int argc, char **argv) {
    js_State *runtime = js_newstate(nullptr, nullptr, 0);
    setupRaylibFuncs(runtime);
    std::string scriptPath = "script.js";
    if (argc > 1) scriptPath = argv[1];
    //R3D_LoadModel("someModel.glb");
    //Create a script template
    if (!std::filesystem::exists(scriptPath)) create_script_template(scriptPath.c_str());
    //Create the function declerations
    if (!std::filesystem::exists(".d.ts")) create_dts_template();
    if (js_dofile(runtime, scriptPath.c_str())) {
        std::cerr << "Error while running script\n";
        return -1;
    }

    // Function on init
    // App start, Raylib not initilaized
    js_dostring(runtime, "onStart();");
    std::cout << "Called onStart" << std::endl;
#ifdef multiplayer
    if (!g_headLessMode)
#endif
    InitWindow(1080, 720, "Hello world");
    R3D_Init(1080, 720);

    R3D_SetAntiAliasingMode(R3D_ANTI_ALIASING_MODE_FXAA);

    // ------------------------------------------------------- SKY
    // Define procedural skybox parameters
    auto skyParams = R3D_PROCEDURAL_SKY_BASE;
    skyParams.groundEnergy = 2.0f;
    skyParams.skyEnergy = 0.5f;
    skyParams.sunEnergy = 0.5f;
    const R3D_Cubemap cubemap = R3D_LoadCubemap("/home/benji/CLionProjects/RaylibPlayGround/Examples/models/citrus_orchard_puresky_2k.hdr", R3D_CUBEMAP_LAYOUT_AUTO_DETECT);
    //const R3D_Cubemap skyProcedural = R3D_GenProceduralSky(512, skyParams);
    R3D_ENVIRONMENT_SET(tonemap.white, 4.0f);

    const R3D_Light light = R3D_CreateLight(R3D_LIGHT_DIR);

    R3D_SetLightDirection(light, (Vector3) {1, -1, 1});
    R3D_SetLightActive(light, true);

    //R3D_ENVIRONMENT_SET(background.color, (Color){0, 0, 255, 255});
    //R3D_ENVIRONMENT_SET(background.sky, skyProcedural);
    R3D_ENVIRONMENT_SET(background.sky, cubemap);

    // Setup environment ambient
    const R3D_AmbientMap ambientMap = R3D_GenAmbientMap(cubemap, R3D_AMBIENT_ILLUMINATION | R3D_AMBIENT_REFLECTION);
    R3D_ENVIRONMENT_SET(ambient.map, ambientMap);
    R3D_ENVIRONMENT_SET(ambient.color, DARKGRAY);
    // ------------------------------------------------------- SKY

    defaultMaterial = R3D_GetDefaultMaterial();
    const auto cube = R3D_GenMeshCube(.25, .25, .25);
    g_vR3DMeshes.push_back(cube);

    // Onready function (after window creation)
    js_dostring(runtime, "onReady();");
    size_t fileModTime = GetFileModTime(scriptPath.c_str());

    while (!WindowShouldClose() || g_headLessMode) {
        if (IsWindowResized()) {
            R3D_SetResolution(GetScreenWidth(), GetScreenHeight());
        }
        //Hot reload
        if (GetFileModTime(scriptPath.c_str()) != fileModTime) {
            std::cout << (js_dofile(runtime, scriptPath.c_str()) ? "Failed to reload" : "Successfully reloaded") << std::endl;
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