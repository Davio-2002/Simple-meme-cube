#pragma once

#include <cstdint>
#include <string>

class Shader {
public:
    // the program ID
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // use active shader
    void Use() const;

    // utility uniform functions
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

	// Check for compilation/linking errors
    static void checkForShaderCompileErrors(const uint32_t& shader, 
                                            const std::string& type);
    static void checkProgramForLinkErrors(const uint32_t& program);

	uint32_t& GetID() { return ID; }

private:
    // Set up shaders
    static void InitializeShader(uint32_t& shader,
                                 const uint32_t type,
                                 const char* code,
                                 const std::string& errorTypeStr);

    uint32_t ID;
};
