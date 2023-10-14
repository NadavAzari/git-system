#ifndef MY_GIT_FILES_H
#define MY_GIT_FILES_H

#include <iostream>
#include <vector>
#include <filesystem>

typedef unsigned char byte;

std::vector<byte> readFileToVector(const std::string& filename);
void walk(std::vector<std::string>& filePaths, std::string start_path = "");

#endif
