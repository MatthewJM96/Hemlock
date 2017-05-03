#pragma once

namespace hemlock {
    namespace io {
        namespace File {
            bool read(const char* filepath, char*& buffer);

            bool readByLine(const char* filepath, std::vector<char*>& buffer);
        }
    }
}
namespace hio = hemlock::io;