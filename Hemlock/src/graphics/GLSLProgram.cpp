#include "stdafx.h"

#include "graphics\GLSLProgram.h"

GLuint hg::GLSLProgram::m_currentProgram = 0;

void hg::GLSLProgram::dispose() {
    if (m_idVS != 0) {
        glDeleteShader(m_idVS);
        m_idVS = 0;
    }

    if (m_idFS != 0) {
        glDeleteShader(m_idFS);
        m_idFS = 0;
    }

    if (m_id != 0) {
        glDeleteProgram(m_id);
        m_id = 0;
        m_isLinked = false;
    }

    AttributeMap().swap(m_attributes);
}

bool hg::GLSLProgram::addShader(hg::ShaderInfo shader) {
    if (!isEditable()) {
        puts("Cannot add a shader to an already linked or not-yet initialised program!");
        return false;
    }

    switch (shader.type) {
        case ShaderType::FRAGMENT:
            if (m_idFS != 0) {
                puts("Cannot add a second fragment shader to the same program!");
                return false;
            }
            break;
        case ShaderType::VERTEX:
            if (m_idVS != 0) {
                puts("Cannot add a second vertex shader to the same program!");
                return false;
            }
            break;
        default:
            puts("Shader type not valid.");
            return false;
    }

    GLuint shaderId = glCreateShader((GLenum)shader.type);
    if (shaderId == 0) {
        puts("Shader could not be created!");
        return false;
    }

    char* buffer;
    hio::readFileToBuffer(shader.filepath, buffer);

    glShaderSource(shaderId, 1, &buffer, nullptr);
    glCompileShader(shaderId);

    delete[] buffer;

    GLint status = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        puts("Shader failed to compile!");
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<char> log(maxLength + 1);
        glGetShaderInfoLog(shaderId, maxLength, nullptr, &log[0]);

        printf("%s\n", &log[0]);

        glDeleteShader(shaderId);
        return false;
    }

    switch (shader.type) {
        case ShaderType::FRAGMENT:
            m_idFS = shaderId;
            break;
        case ShaderType::VERTEX:
            m_idVS = shaderId;
            break;
    }

    return true;
}

bool hg::GLSLProgram::addShaders(const char* fragmentShaderPath, const char* vertexShaderPath) {
    return addShader({ (char*)fragmentShaderPath, ShaderType::FRAGMENT }) && addShader({ (char*)vertexShaderPath, ShaderType::VERTEX });
}

bool hg::GLSLProgram::link() {
    if (!isEditable()) {
        puts("Cannot link an already linked or not-yet initialised program!");
        return false;
    }

    if (!m_idVS || !m_idFS) {
        puts("Both a vertex and fragment shader must be provided to link!");
        return false;
    }

    glAttachShader(m_id, m_idVS);
    glAttachShader(m_id, m_idFS);

    glLinkProgram(m_id);

    glDetachShader(m_id, m_idVS);
    glDetachShader(m_id, m_idFS);

    glDeleteShader(m_idVS);
    glDeleteShader(m_idFS);
    m_idVS = 0;
    m_idFS = 0;

    GLint status = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &status);
    m_isLinked = status == GL_TRUE;
    if (!m_isLinked) {
        puts("Shader failed to link!");
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        char* log = new char[maxLength];
        glGetProgramInfoLog(m_id, maxLength, nullptr, log);

        printf("%s\n", log);
        return false;
    }
    return true;
}

void hg::GLSLProgram::setAttribute(char* name, GLuint index) {
    if (!isEditable()) {
        puts("Attributes may not be set before program is created or after is has been linked.\n");
        return;
    }

    glBindAttribLocation(m_id, index, name);
    m_attributes[name] = index;
}

void hg::GLSLProgram::setAttributes(std::map<char*, GLuint> attributes) {
    if (!isEditable()) {
        puts("Attributes may not be set before program is created or after is has been linked.\n");
        return;
    }

    for (auto& attribute : attributes) {
        glBindAttribLocation(m_id, attribute.second, attribute.first);
        m_attributes[attribute.first] = attribute.second;
    }
}

GLuint hg::GLSLProgram::getUniformLocation(char* name) {
    // TODO(Matthew): If we add shaders from source instead of filepath, could parse location of uniforms that explicitly set it using "layout(location = X)".
    if (!isLinked()) {
        puts("Cannot find location of uniform until the program has been linked!");
        return GL_INVALID_OPERATION;
    }

    GLint location = glGetUniformLocation(m_id, name);
    if (!location) {
        printf("Uniform %s not found in shader!", name);
    }
    return location;
}

void hg::GLSLProgram::use() {
    if (!isInUse()) {
        glUseProgram(m_id);
        m_currentProgram = m_id;
    }
}

void hg::GLSLProgram::unuse() {
    if (m_currentProgram != 0) {
        glUseProgram(0);
        m_currentProgram = 0;
    }
}
