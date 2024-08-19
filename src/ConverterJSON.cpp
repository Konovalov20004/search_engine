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
    }
    else
        std::cout << "Error. Configs file not found" << std::endl;
    file_config.close();
    file_request.close();
    std::cout << config_file["config"]["name"] << std::endl;
}

std::vector<std::string> ConverterJSON::GetTextDocument(){
    std::vector<std::string> text_document;

    config_file.clear();
    file_config.open(file_patch + "/search_engine/json/config.json");
    config_file = json::parse(file_config);

    for (auto &file : config_file["files"]){
        std::fstream openFile;
        openFile.open(file_patch + "/search_engine" + (std::string)file);
        if(openFile.is_open()){
            char text[1000000];
            openFile.get(text,sizeof(text));
            text_document.push_back((std::string)text);
        } else
            std::cout << "file not found" << std::endl;
        openFile.close();
    }

    file_config.close();
    return text_document;
}

std::vector<std::string> ConverterJSON::GetRequest() {
    file_request.open(file_patch + "/search_engine/json/requests.json");
    request_file = json::parse(file_request);
    std::vector<std::string> request;
    for(auto &requestWords :request_file["requests"]) {
        request.push_back((std::string)requestWords);
    }
    file_request.close();
    return request;
}

int ConverterJSON::ResponseLimit(){
    int response_max;
    config_file["config"]["max_responses"] != config_file["config"]["max_responses"].empty() ?
        response_max = config_file["config"]["max_responses"] : response_max = 5;
    return response_max;
}

void ConverterJSON::putAnswer(std::vector<std::vector<RelativeIndex>> answer) {
    Index index;
    json answers_file;

    file_answers.open(file_patch + "/search_engine/json/answers.json");
    //file_answers.clear();

    for (auto& request : answer) {

        int response_max = ResponseLimit();
//!!!
        if (request.size() == 1) {
            answers_file["answers"]["request" + index.getIndex(1)] += {"result", true};
            answers_file["answers"]["request" + index.getIndex(0)] += {
                {"docid", request[0].doc_id}, { "rank", request[0].rank }};
        }
        else if (request.empty()) {
            answers_file["answers"]["request" + index.getIndex(1)]["result"] = false;
        }
        else {
            answers_file["answers"]["request" + index.getIndex(1)]["result"] = true;
            for (auto& line : request) {
                answers_file["answers"]["request" + index.getIndex(0)]["relevance"] += {
                    {"docid", line.doc_id}, { "rank", line.rank }};

                if (!--response_max)
                    break;
            }
        }
    }
    if (file_answers.is_open()) {
        file_answers << answers_file;
        std::cout << "Answer done" << std::endl;
    }
    else {
        std::cout << "error create file" << std::endl;
    }

    file_answers.close();
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
