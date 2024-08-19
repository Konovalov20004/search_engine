#include "search_engine.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search (const std::vector<std::string>& queries_input) {
	
	std::vector<std::vector<RelativeIndex>> result;
	std::mutex mutex;

	InvertedIndex* index = _index;

	std::vector<std::thread*> threads;
	std::thread* thread;
	int id_thread{0};

	for (auto& queries : queries_input) {
		
		id_thread++;
		thread = new std::thread([&result, &index, &mutex, &queries, &id_thread]() {
			
			int my_id = id_thread;
			
			mutex.lock();
			std::map<size_t, size_t> tmp_map;
			std::istringstream repeat(queries);
			std::unordered_set<std::string> no_repeat;
			std::string word;
			do {
				repeat >> word;
				if (no_repeat.find(word) == no_repeat.end())
					no_repeat.insert(word);
			} while (repeat);

			for (auto& el : no_repeat) {
				std::vector<Entry> tmp_vec(index->GetWordCount(el));
				for (auto& doc : tmp_vec) {
					if (tmp_map.find(doc.doc_id) == tmp_map.end()) {
						tmp_map.emplace(doc.doc_id, doc.count);
					}
					else {
						tmp_map.find(doc.doc_id)->second += doc.count;
					}
				}
			}

			RelativeIndex ri{};
			std::vector<RelativeIndex> vec_relative_index{};
			if (!tmp_map.empty()) {
				std::multimap<float, size_t> sorted_map;
				for (auto& el : tmp_map) {
					sorted_map.emplace(el.second, el.first);
				}
				int max_relative = sorted_map.rbegin()->first;
				for (auto i = sorted_map.rbegin(); i != sorted_map.rend(); i++) {
					ri.doc_id = i->second;
					ri.rank = i->first / max_relative;
					vec_relative_index.push_back(ri);
				}
				result.push_back(vec_relative_index);
			}
			else {
				result.push_back(vec_relative_index);
			}
			mutex.unlock();
		});

		std::cout << id_thread << ". request\t\"" << queries << "\"" << std::endl;
 		threads.push_back(thread);

	}

	for (auto el : threads) {
		if (el->joinable()) {
			el->join();
			delete el;
		}
	}

	return result;
}
