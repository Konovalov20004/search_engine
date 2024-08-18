#include "ConverterJSON.h"

#include <iostream>
#include <source_location>
using json = nlohmann::json;

ConverterJSON::ConverterJSON(){
    std::source_location loc = std::source_location::current();
    file_patch = loc.file_name();
    file_patch = file_patch.substr(0, file_patch.rfind("search_engine"));
    file_config.open(file_patch + "/search_engine/json/config.json");
    file_request.open(file_patch + "/search_engine/json/requests.json");
    if(file_config.is_open() && file_request.is_open()) {
        config_file = json::parse(file_config);
        request_file = json::parse(file_request);
    } else
        throw "Error. Config file not found";
    file_config.close();
    file_request.close();
    std::cout << config_file["config"]["name"] << std::endl;
}

std::vector<std::string> ConverterJSON::GetTextDocument(){
    std::vector<std::string> text_document;


    if (!text_document.empty())
        return text_document;

    for (auto &file : config_file["files"]){
        std::fstream openFile;
        openFile.open(file_patch + "/search_engine" + (std::string)file);
        if(openFile.is_open()){
            char text[1000000];
            openFile.get(text,sizeof(text));
            text_document.push_back((std::string)text);
        } else
            throw "file not found";
        openFile.close();
    }
    return text_document;
}

std::vector<std::string> ConverterJSON::GetRequest() {
    std::vector<std::string> request;
    for(auto &requestWords :request_file["requests"]) {
        request.push_back((std::string)requestWords);
    }
    return request;
}

int ConverterJSON::ResponseLimit(){
    return config_file["config"]["max_responses"];
}

void ConverterJSON::putAnswer(std::vector<std::vector<RelativeIndex>> answer) {
    Index index;
    int response_max{};
    config_file["config"]["max_responses"] != config_file["config"]["max_responses"].empty() ?
       response_max = config_file["config"]["max_responses"] : response_max = 5;

    file_answers.open(file_patch + "/search_engine/json/answers.json");

    for (auto& request : answer) {

        if (request.size() == 1) {
            answers_file["answers"]["request" + index.getIndex(1)] += {"result", true};
            answers_file["answers"]["request" + index.getIndex(0)] += {
                {"docid", request[0].doc_id}, { "rank", request[0].rank }};
        } else if (request.empty()) {
            answers_file["answers"]["request" + index.getIndex(1)]["result"] = false;
        }
        else {
            answers_file["answers"]["request" + index.getIndex(1)]["result"] += true;
            for (auto& line : request) {
                answers_file["answers"]["request" + index.getIndex(0)]["relevance"] += {
                    {"docid", line.doc_id}, { "rank", line.rank }};
            
                if (!response_max--)
                    break;
            }
        }
    }
    if (file_answers.is_open())
        file_answers << answers_file;
    else
        std::cout << "error create file" << std::endl;
}

Index::Index() {
    a = 0;
    b = 0;
    c = 0;
}

std::string Index::getIndex(int move) {
    std::string index;
    if (move == 1) {
        if (c == 9) {
            c = 0;
            if (b == 9) {
                b = 0;
                if (a == 9) {
                    return "-1";
                }else
                    a++;
            }else
                b++;
        }else
            c++;
    }
    else if (move == -1) {
        if (c == 0) {
            c = 0;
            if (b == 0) {
                b = 0;
                if (a == 0) {
                    return "-1";
                }else
                    a--;
            }else
                b--;
        }else
            c--;
    }
    index = std::to_string(a) + std::to_string(b) + std::to_string(c);
    return index;
}
