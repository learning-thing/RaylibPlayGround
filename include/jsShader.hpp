//
// Created by benji on 02.06.26.
//

#ifndef RAYLIBPLAYGROUND_JSSHADER_HPP
#define RAYLIBPLAYGROUND_JSSHADER_HPP

#include <fstream>

#include "raylib.h"

inline const char *baseFragmentShader =
"#version 120\n"
"// Input vertex attributes (from vertex shader)\n"
"varying vec2 fragTexCoord;\n"
"varying vec4 fragColor;\n"
"// Input uniform values\n"
"uniform sampler2D texture0;\n"
"uniform vec4 colDiffuse;\n"
"\n"
"// NOTE: Add your custom variables here\n"
"uniform vec2 resolution = vec2(800, 450);\n"
"\n"
"void main() {\n"
"    // Texel color fetching from texture sampler\n"
"    vec4 texelColor = texture2D(texture0, fragTexCoord);\n"
"\n"
"    // NOTE: Implement here your fragment shader code\n"
"\n"
"    gl_FragColor = texelColor*colDiffuse;\n"
"}\n";

inline const char *baseVertexShader =
"#version 120\n"
"\n"
"// Input vertex attributes\n"
"attribute vec3 vertexPosition;\n"
"attribute vec2 vertexTexCoord;\n"
"attribute vec3 vertexNormal;\n"
"attribute vec4 vertexColor;\n"
"\n"
"// Input uniform values\n"
"uniform mat4 mvp;\n"
"\n"
"// Output vertex attributes (to fragment shader)\n"
"varying vec2 fragTexCoord;\n"
"varying vec4 fragColor;\n"
"\n"
"// NOTE: Add your custom variables here\n"
"\n"
"void main() {\n"
"    // Send vertex attributes to fragment shader\n"
"    fragTexCoord = vertexTexCoord;\n"
"    fragColor = vertexColor;\n"
"\n"
"    // Calculate final vertex position\n"
"    gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
"}\n";

class jsShader {
    Shader m_sShader;
    size_t fileModTime;
    const char *m_sVertexPath;
    const char *m_sFragmentPath;
    void reload();
public:
    jsShader(const char *vertexPath, const char *fragmentPath) : m_sShader(LoadShader(vertexPath, fragmentPath)), m_sVertexPath(vertexPath), m_sFragmentPath(fragmentPath) {
        fileModTime = GetFileModTime(fragmentPath);
    }
    int getUniformLoc(const char *name) const;
    void setUniform(int loc, float a) const;
    void setUniform(int loc, Vector3 a) const;
    void begin();
};


#endif //RAYLIBPLAYGROUND_JSSHADER_HPP
