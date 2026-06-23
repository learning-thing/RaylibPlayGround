//
// Created by benji on 02.06.26.
//

#include "jsShader.hpp"

#include <iostream>

int jsShader::getUniformLoc(const char *name) const {
    const int loc = GetShaderLocation(m_sShader, name);;
    std::cout << "Shader " << m_sFragmentPath <<  " location of \"" << name << "\" is " << loc << std::endl;
    return loc;
}

void jsShader::begin() {
    const auto fsNowModTime = GetFileModTime(m_sFragmentPath);
    const auto vsNowModTime = GetFileModTime(m_sVertexPath);

    // std::cout << nowModTime << std::endl;
    if (fsFileModTime != fsNowModTime || vsFileModTime != vsNowModTime) {
        reload();
        std::cout << "Shader reload" << std::endl;
        fsFileModTime = fsNowModTime;
        vsFileModTime = vsNowModTime;
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
