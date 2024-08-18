#include <iostream>
#include <vector>
#include <string>
#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include <nlohmann/json.hpp>
#include <map>
#include <thread>
#include <mutex>

struct Entry {
    size_t doc_id, count;

    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();
    void UpdateDocumentBase(std::vector<std::string> docs);
    std::vector<Entry> GetWordCount(const std::string& word);
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};


#endif // INVERTEDINDEX_H
