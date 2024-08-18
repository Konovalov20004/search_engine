#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "search_engine.h"

int main()
{
    ConverterJSON a;
    InvertedIndex b;
    b.UpdateDocumentBase(a.GetTextDocument());
    SearchServer c(b);
    a.putAnswer(c.search(a.GetRequest()));

    return 0;
    
}