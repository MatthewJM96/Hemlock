#include "stdafx.h"

#include "io/FileLoader.h"

bool hio::File::read(const char* filepath, char*& buffer) {
    std::ifstream file(filepath, std::ios::binary);
    if (file.fail()) {
        perror(filepath);
        return false;
    }

    file.seekg(0, std::ios::end);
    std::streamoff size = file.tellg();
    file.seekg(0, std::ios::beg);
    size -= file.tellg();

    buffer = new char[size + 1];
    file.read(buffer, size);
    buffer[size] = '\0';

    file.close();
    return true;
}

bool hio::File::readByLine(const char* filepath, std::vector<char*>& buffer) {
    std::ifstream file(filepath, std::ios::binary);
    if (file.fail()) {
        perror(filepath);
        return false;
    }

    buffer.clear();

    std::string line;
    while (std::getline(file, line)) {
        line += "\n";

        char* linebuffer = new char[line.size() + 1];
        memcpy(linebuffer, (void*)line.c_str(), line.size());
        linebuffer[line.size()] = '\0';

        buffer.push_back(linebuffer);
    }

    file.close();
    return true;
}
