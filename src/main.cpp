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

    while (true)
    {
        std::cin >> command;
        if (command == "search")
            a.putAnswer(c.search(a.GetRequest()));
        else if (command == "updatebase") {
            b.UpdateDocumentBase(a.GetTextDocument());
        }
        else if (command == "exit")
            exit(0);
        else {
            std::cout << "Command not found" << std::endl;
            std::cout << "M.b. you will\n- search \n- updatebase" << std::endl;
        }
    }

    return 0;
    
}