#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "search_engine.h"

int main()
{   
    std::string command;
    
    ConverterJSON a;
    InvertedIndex b;
    b.UpdateDocumentBase(a.GetTextDocument());
    SearchServer c(b);

    std::cout << "Use command\n1 - search\n2 - updatebase\n0 - exit" << std::endl;

    while (true)
    {
        std::cin >> command;
        if (command == "search" || command == "1")
            a.putAnswer(c.search(a.GetRequest()));
        else if (command == "updatebase" || command == "2") {
            b.UpdateDocumentBase(a.GetTextDocument());
        }
        else if (command == "exit"|| command == "0")
            exit(0);
        else {
            std::cout << "Command not found" << std::endl;
            std::cout << "M.b. you will\n- search \n- updatebase" << std::endl;
        }
    }

    return 0;
    
}