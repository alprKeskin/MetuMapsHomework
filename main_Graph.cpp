#include "Graph.h"
#include "GraphExceptions.h"
#include <iostream>
#include "Graph.cpp"

int main()
{
    Graph g;

    g.InsertVertex("A");
    g.InsertVertex("B");
    g.InsertVertex("C");
    g.InsertVertex("D");
    g.InsertVertex("E");
    g.InsertVertex("F");
    g.InsertVertex("J");

    g.ConnectVertices("A", "B", 5);
    g.ConnectVertices("A", "C", 1);
    g.ConnectVertices("B", "C", 2);
    g.ConnectVertices("B", "D", 3);
    g.ConnectVertices("C", "E", 10);
    g.ConnectVertices("D", "E", 8);
    g.ConnectVertices("D", "F", 1);
    g.ConnectVertices("E", "F", 7);
    g.ConnectVertices("F", "J", 16);


    std::vector<StringPair> edgesToBeMasked;
    StringPair stringPairDF("D", "F");
    StringPair stringPairEF("E", "F");
    StringPair stringPairAC("A", "C");
    StringPair stringPairAB("A", "B");
    edgesToBeMasked.push_back(stringPairDF);
    edgesToBeMasked.push_back(stringPairEF);
    //edgesToBeMasked.push_back(stringPairAC);
    //edgesToBeMasked.push_back(stringPairAB);

    g.MaskEdges(edgesToBeMasked);
    
    std::vector<int> orderedVertexIdList;

    g.ShortestPath(orderedVertexIdList, "A", "K");

    for (int i = 0; i < orderedVertexIdList.size(); i++) {
        std::cout << g.VertexName(orderedVertexIdList[i]) << std::endl;
    }

    std::cout << "length of orderedVertexIdList is: " << orderedVertexIdList.size() << std::endl;



    /*
    try
    {
        g.ConnectVertices("A", "J", 9);
    }
    catch(const TooManyEdgeOnVertexExecption& e)
    {
        std::cout << e.what() << '\n';
    }
    */

    return 0;
}