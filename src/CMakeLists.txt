cmake_minimum_required(VERSION 3.8)

set(Src
	"search_engine.cpp"
	"main.cpp"
	"ConverterJSON.cpp"
	"InvertedIndex.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(search_engine ${Src})

target_include_directories(search_engine PUBLIC ${MY_INCLUDE_DIR})
target_link_libraries(search_engine nlohmann_json::nlohmann_json)