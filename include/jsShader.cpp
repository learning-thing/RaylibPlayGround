//
// Created by benji on 02.06.26.
//

#include "jsShader.hpp"

#include <iostream>

int jsShader::getUniformLoc(const char *name) const {
    return GetShaderLocation(m_sShader, name);
}

void jsShader::begin() {
    const auto nowModTime = GetFileModTime(m_sFragmentPath);
    // std::cout << nowModTime << std::endl;
    if (fileModTime != nowModTime) {
        reload();
        std::cout << "Shader reload" << std::endl;
        fileModTime = nowModTime;
    }
    BeginShaderMode(m_sShader);
}

void jsShader::setUniform(const int loc, const float a) const {
    SetShaderValue(m_sShader, loc, &a, SHADER_UNIFORM_FLOAT);
}

void jsShader::setUniform(const int loc, const Vector3 a) const {
    SetShaderValue(m_sShader, loc, &a, SHADER_UNIFORM_VEC3);
}

void jsShader::reload() {
    UnloadShader(m_sShader);
    const Shader newShader = LoadShader(m_sVertexPath, m_sFragmentPath);
    m_sShader = newShader;
}
