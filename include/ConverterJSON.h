#ifndef CONVETRERJSON_H
#define CONVETRERJSON_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "search_engine.h"

using json = nlohmann::json;

struct Index {
    size_t a, b, c;
    Index();
    std::string getIndex(int move);
};

class ConverterJSON {
public:
    ConverterJSON();
    std::vector<std::string> GetTextDocument();
    int ResponseLimit();
    std::vector<std::string> GetRequest();
    void putAnswer(std::vector<std::vector<RelativeIndex>> answer);
private:
    std::string file_patch;
    std::ifstream file_config;
    std::ifstream file_request;
    std::ofstream file_answers;
    json config_file;
    json request_file;
};

#endif // CONVETRERJSON_H
