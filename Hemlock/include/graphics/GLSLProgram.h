#pragma once

#ifndef HEMLOCK_USING_PCH
#include "types.h"
#endif

namespace hemlock {
    namespace graphics {
        enum class ShaderType {
            FRAGMENT = GL_FRAGMENT_SHADER,
            VERTEX = GL_VERTEX_SHADER
        };

        struct ShaderInfo {
            char* filepath;
            ShaderType type;
        };

        class GLSLProgram {
        public:
            typedef std::map<const char*, GLuint> AttributeMap;

            GLSLProgram() {};
            ~GLSLProgram() {};

            void init() {
                if (isInitialised()) return;
                m_id = glCreateProgram();
            }
            void dispose();

            GLuint getId() {
                return m_id;
            }

            bool isInitialised() const {
                return m_id != 0;
            }
            bool isLinked() const {
                return m_isLinked;
            }
            bool isEditable() const {
                return !isLinked() && isInitialised();
            }
            bool isInUse() const {
                return m_id == m_currentProgram;
            }

            // TODO(Matthew): Acquire source instead of filepath here and have separate utility class to read in source from a file? Would allow for programatically created shaders.
            bool addShader(ShaderInfo shader);
            bool addShaders(const char* fragmentShaderPath, const char* vertexShaderPath);

            bool link();

            void setAttribute(const char* name, GLuint index);
            void setAttributes(std::map<const char*, GLuint> attributes);

            GLuint getAttributeLocation(const char* name) {
                return m_attributes.at(name);
            }
            GLuint getUniformLocation(const char* name);

            void use();
            static void unuse();

            static GLuint getCurrentProgram() {
                return m_currentProgram;
            }
        private:
            void shaderError();

            GLuint m_id   = 0;
            GLuint m_idVS = 0;
            GLuint m_idFS = 0;

            bool m_isLinked = false;

            AttributeMap m_attributes;

            static GLuint m_currentProgram;
        };
    }
}
namespace hg = hemlock::graphics;
