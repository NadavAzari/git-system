#include "files.h"
#include "repo/repo.h"

void walk(std::vector<std::string>& filePaths, std::string start_path) {
    std::filesystem::path currentDirectory = start_path;
    if(start_path == "") {
        currentDirectory = std::filesystem::current_path();
    }

    if (!std::filesystem::is_directory(currentDirectory))
        return;

    for (const auto& entry : std::filesystem::directory_iterator(currentDirectory)) {
        if (std::filesystem::is_directory(entry)) {
            if(entry.path().filename() == GIT_EXTENSION) {
                continue;
            }
            walk(filePaths, entry.path().string());
        } else if (std::filesystem::is_regular_file(entry)) {
            filePaths.push_back(std::filesystem::relative(entry.path().string(), std::filesystem::current_path()));
        }
    }
}

std::vector<byte> readFileToVector(const std::string& filename) {
    std::ifstream inputFile(filename, std::ios::binary);

    if (!inputFile.is_open()) {
        //TODO: Error
    }

    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<byte> fileData(fileSize);

    inputFile.read(reinterpret_cast<char*>(fileData.data()), fileSize);

    inputFile.close();

    return fileData;
}