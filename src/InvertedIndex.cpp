#include "InvertedIndex.h"

InvertedIndex::InvertedIndex(){
    freq_dictionary;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> docs)  {
    freq_dictionary.clear();
    std::map<std::string,std::vector<Entry>> tmp_map;

    for (int i = 0; i < docs.size(); i++) {
            Entry entry{};
            entry.count = 1;
            entry.doc_id = i;
            std::vector<Entry> vectorEntry;
            vectorEntry.push_back(entry);
            std::string doc = docs[i];
            while (!doc.empty()) {
                std::string word = doc.substr(0, doc.find(" "));
                if (word.empty());
                else if (tmp_map.find(word) == tmp_map.end()) {
                    tmp_map.emplace(std::make_pair(word, vectorEntry));
                }
                else {
                    bool id_doc{};
                    for (auto& el : tmp_map.find(word)->second) {
                        if (el.doc_id == i) {
                            el.count++;
                            id_doc = 1;
                        }
                    }
                    if (!id_doc)
                        tmp_map.find(word)->second.push_back(entry);
                }

             doc.erase(0, word.length() + 1);
            }
    }
    freq_dictionary.merge(tmp_map);
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {

    std::map<std::string, std::vector<Entry>>::iterator it = freq_dictionary.find(word);
    std::vector<Entry> empty;
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    else {
        return empty;
    }
}

