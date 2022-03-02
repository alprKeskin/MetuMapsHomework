#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{}

Graph::Graph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if(i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if(!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string& vertexName) {
    // TODO
    // traverse the vertex list of the graph
    for (int i = 0; i < vertexList.size(); i++) {
        // if a vertex with the same name has been found in the vertexList of the graph
        if (vertexList[i].name == vertexName) {
            // give exception error
            throw DuplicateVertexNameException();
            // return the process
            return;
        }
        // if it is not a vertex with the same name
        else {
            continue;
        }
    }
    // there is no any vertex with the same name in the vertex list of the graph
    // create a graph vertex ,with the given name, without any edges
    struct GraphVertex newGraphVertex;
    // set edgeCount to 0
    newGraphVertex.edgeCount = 0;
    // put the name of the vertex
    newGraphVertex.name = vertexName;
    // add this graph vertex to vertex list of the graph
    vertexList.push_back(newGraphVertex);
}

bool Graph::ConnectVertices(const std::string& fromVertexName, const std::string& toVertexName, int weight) {
    // TODO
    int fromVertexNameIndex;
    int toVertexNameIndex;
    bool fromVertexNameExist = false;
    bool toVertexNameExist = false;

    if (fromVertexName == toVertexName) {
        return false;
    }

    
    // find the vertex with the fromVertexName and toVertexName in the vertexList by traversing the whole list
    for (int i = 0; i < vertexList.size(); i++) {
        // if we have found the vertex with the fromVertexName
        if (vertexList[i].name == fromVertexName) {
            // assign the index
            fromVertexNameIndex = i;
            // update the existence info 
            fromVertexNameExist = true;
        }
        // if we have found the vertex with the toVertexName
        else if (vertexList[i].name == toVertexName) {
            // assign the index
            toVertexNameIndex = i;
            // update the existence info 
            toVertexNameExist = true;
        }
        // if this is not a vertex including given names
        else {
            // continue searching
            continue;
        }
    }
    

    // if at least one of the vertices with the given name does not exist
    if (fromVertexNameExist == false || toVertexNameExist == false) {
        // give vertex not found exception
        throw VertexNotFoundException();
        // return false
        return false;
    }

    // if at least one of these vertices already has the maximum number of edges 
    if (vertexList[fromVertexNameIndex].edgeCount == MAX_EDGE_PER_VERTEX || vertexList[toVertexNameIndex].edgeCount == MAX_EDGE_PER_VERTEX) {
        // give too many edge on vertex error
        throw TooManyEdgeOnVertexExecption();
        // return false
        return false;
    }

    // traverse the whole edgeList
    for (int i = 0; i < edgeList.size(); i++) {
        // if the edge has the vertexId0 and vertexId1 same as fromVertexNameIndex and toVertexNameIndex
        if (edgeList[i].vertexId0 == fromVertexNameIndex && edgeList[i].vertexId1 == toVertexNameIndex || edgeList[i].vertexId0 == toVertexNameIndex && edgeList[i].vertexId1 == fromVertexNameIndex) {
            // then, these two vertices are already connected to each other
            return false;
        }
        // if the edge is not the one that we are looking for
        else {
            // continue searching
            continue;
        }
    }
    // these two vertices are not connected to each other, at this point.

    // insert the new generated edge's id (its index in edgeList, edgeList.size().) at the end of their edgeIds array
    vertexList[fromVertexNameIndex].edgeIds[vertexList[fromVertexNameIndex].edgeCount] = edgeList.size();
    vertexList[toVertexNameIndex].edgeIds[vertexList[toVertexNameIndex].edgeCount] = edgeList.size();

    // increase the edgeCount information of the vertices
    vertexList[fromVertexNameIndex].edgeCount++;
    vertexList[toVertexNameIndex].edgeCount++;

    // create a graph edge
    struct GraphEdge newEdge;
    newEdge.weight = weight;
    newEdge.masked = false;
    newEdge.vertexId0 = fromVertexNameIndex;
    newEdge.vertexId1 = toVertexNameIndex;

    // add this graph edge at the end of the edge list
    edgeList.push_back(newEdge);

    // connection has been carried out successfully
    return true;
}

// Helper functions of ShortestPath function
int Graph::FindVertexId(const std::string& vertexName) const {
    // traverse the vertexList
    for (int i = 0; i < vertexList.size(); i++) {
        // if this is the vertex with the given name
        if (vertexList[i].name == vertexName) {
            // return its index
            return i;
        }
        // if this is not the vertex with the given name
        else {
            // continue searching
            continue;
        }
    }
    // there is no any vertex with the given name
    return -1;
}

bool Graph::IsAllFalse(const std::vector<bool> givenVector) const {
    for (int i = 0; i < givenVector.size(); i++) {
        if (givenVector[i] == true) {
            return false;
        }
        else {
            continue;
        }
    }
    return true;
}

int Graph::FindMinimumUnvisitedVertex(const std::vector<int> distances, const std::vector<bool> unvisited) const {
    int minDistance = LARGE_NUMBER;
    int minIndex = -1;
    // traverse all vertices
    for (int i = 0; i < unvisited.size(); i++) {
        // if this vertex has been visited
        if (unvisited[i] == false) {
            continue;
        }
        // if this vertex has not been visited
        else {
            // if its distance is less than current min distance
            if (distances[i] <= minDistance) {
                // set it as new min
                minDistance = distances[i];
                minIndex = i;
            }
            else {
                continue;
            }
        }
    }
    return minIndex;
}

bool Graph::ShortestPath(std::vector<int>& orderedVertexIdList, const std::string& from, const std::string& to) const {
    // TODO
    // create a vector of size of the number of total vertices
    // this vector will hold the information if the vertex which has the same index in the vertexList was unvisited before
    // all the values will be initialized as true since they were unvisited before
    std::vector<bool> unvisited(vertexList.size(), true);
    // create a vector of size of the number of total vertices
    // this vector will hold the information that the distance of the vertex which is indexed same with the vertexList
    // all the values will be initialized to infinity
    std::vector<int> distances(vertexList.size(), LARGE_NUMBER);
    // create a vector of size of the number of total vertices
    // this vector will hold the information of the vertexId we have visited before the vertex which is indexed same with the vertexList
    // all the values will be initialized to -1 as a placeholder
    std::vector<int> previous(vertexList.size(), -1);

    // save the vertexId of from
    int fromIndex = FindVertexId(from);
    // save the vertexId of to
    int toIndex = FindVertexId(to);
    // if one of the vertices does not exist
    if (fromIndex == -1 || toIndex == -1) {
        // give vertex not found error
        throw VertexNotFoundException();
        // terminate the process
        return false;
    }
    // keep the information if we have reached to the aimed index
    bool reachedToIndex = false;

    // set the distance of from to 0
    distances[fromIndex] = 0;
    // while all vertices are not visited
    while (!IsAllFalse(unvisited)) {
        // find the unvisited vertex (index) with the minimum distance
        int newVertexIndex = FindMinimumUnvisitedVertex(distances, unvisited);

        // if we could not have reached before to "to" and the distance of newVertexIndex is infinity
        if ( (distances[newVertexIndex] >= LARGE_NUMBER) && !reachedToIndex) {
            // then, there is no a valid path from "from" to "to"
            // terminate the process
            return false;
        }

        // if we reached to "to" for the first time
        // since the conditional jump did not enter to the if above, then distance of to index is not infinity
        if (newVertexIndex == toIndex) {
            // set reached to index data true
            reachedToIndex = true;
        }

        // find all unmasked edges of newVertex
        for (int i = 0; i < edgeList.size(); i++) {
            // if one of the vertices of this edge is new vertex and this edge is unmasked
            if ( (edgeList[i].vertexId0 == newVertexIndex || edgeList[i].vertexId1 == newVertexIndex) && edgeList[i].masked != true ) {
                // calculate the new distance of this neighbour
                int newDistance = distances[newVertexIndex] + edgeList[i].weight;
                // save the index of this neighbour
                int neighbourIndex;
                // assign the neighbour index correctly
                if (edgeList[i].vertexId0 == newVertexIndex) {
                    neighbourIndex = edgeList[i].vertexId1;
                }
                else {
                    neighbourIndex = edgeList[i].vertexId0;
                }
                // if this neighbour index has been visited before
                if (unvisited[neighbourIndex] == false) {
                    // do nothing with this vertex
                    // continue searching other unmasked edges
                    continue;
                }
                // if new distance is less than the old one
                if (newDistance < distances[neighbourIndex]) {
                    // update the distance of that neighbour vertex
                    distances[neighbourIndex] = newDistance;
                    // update the previous of that vertex
                    previous[neighbourIndex] = newVertexIndex;
                }
                // if new distance is not less than the old one  
                else {
                    continue;
                }
            }
            // if this edge does not belong to new vertex
            else {
                // continue searching
                continue;
            }
        }
        // set newVertex as visited
        unvisited[newVertexIndex] = false;
    }

    
    // create an array to put shortest path in the reverse order
    std::vector<int> reverseShortestPath;
    // set your cursor to traverse shortest path in the reverse order
    int currentIndex = toIndex;
    // while current index is not from index (starting vertex)
    while (currentIndex != fromIndex) {
        // append that index to the reverseShortestPath
        reverseShortestPath.push_back(currentIndex);
        // take a step for current index
        currentIndex = previous[currentIndex];
    }
    // when current index is from index, append it also
    reverseShortestPath.push_back(currentIndex);

    // build ordered vertex ıd list in the correct order
    for (int i = 1; i <= reverseShortestPath.size(); i++) {
        orderedVertexIdList.push_back(reverseShortestPath[reverseShortestPath.size() - i]);
    }

    // since shortest path has been found, return true
    return true;
}

// Helper functions of MultipleShortPaths function
int Graph::FindEdgeBetweenTwoVertices(const int vertexId0, const int vertexId1) const {
    // traverse the edge list
    for (int i = 0; i < edgeList.size(); i++) {
        // if this edge is in between given vertices
        if (edgeList[i].vertexId0 == vertexId0 && edgeList[i].vertexId1 == vertexId1 || edgeList[i].vertexId0 == vertexId1 && edgeList[i].vertexId1 == vertexId0) {
            // return the index of this edge
            return i;
        }
        else {
            // continue searching
            continue;
        }
    }
    // there is no edge between given vertices
    return -1;
}

int Graph::FindHighestWeightedEdgeIndexOfPath(const std::vector<int> lastFoundPath) const {
    int highestWeight = -1;
    int highestWeightIndex = -1;

    // traverse the whole shortest path list
    for (int i = 0; i < lastFoundPath.size() - 1; i++) {
        // find the index (in the edge list) of the edge between two successor vertices
        int edgeIndexBetweenVertices = FindEdgeBetweenTwoVertices(lastFoundPath[i], lastFoundPath[i + 1]);
        // if the weight of this edge is greater than current highestWeight
        if (edgeList[edgeIndexBetweenVertices].weight > highestWeight) {
            // update highest weight information
            highestWeight = edgeList[edgeIndexBetweenVertices].weight;
            highestWeightIndex = edgeIndexBetweenVertices;
        }
        // if the weight of this edge is less than or equal to current highestWeight
        else {
            // pass to the next edge
            continue;
        }
    }

    // return the edge list index of highest weighted index in th shortest path
    return highestWeightIndex;
}

int Graph::MultipleShortPaths(std::vector<std::vector<int> >& orderedVertexIdList, const std::string& from, const std::string& to, int numberOfShortestPaths) {
    // TODO
    int highestWeightedEdgeIndexOfPath;
    // keep the number of founded paths
    int foundedPathsNumber = 0;
    // save vertex ids (index) of "from" and "to"
    int fromIndex = FindVertexId(from);
    int toIndex = FindVertexId(to);
    // if one of "from" or "to" do not exist
    if (fromIndex == -1 || toIndex == -1) {
        // give vertex not found error
        throw VertexNotFoundException();
    }
    // unmask all the edges
    UnMaskAllEdges();
    // create a vector to keep last found shortest path
    std::vector<int> shortestPathVerticesList;
    // find the shortest path and add it to
    // if shortest path exists
    if ( ShortestPath(shortestPathVerticesList, from, to) ) {
        // increase the number of founded paths since we have just found one
        foundedPathsNumber++;
        // add the last found path to the ordered vertex ıd list
        orderedVertexIdList.push_back(shortestPathVerticesList);
        // clear shortest path vertices list for future use
        // !!!!!!!!!!! pass by reference ile ordered vertex ıd list'e geçiyorsa sorun !!!!!!!!!!!
        shortestPathVerticesList.clear();
        // until number of shortest paths is reached
        while (foundedPathsNumber < numberOfShortestPaths) {
            // find highest weighted edge in the last found path
            highestWeightedEdgeIndexOfPath = FindHighestWeightedEdgeIndexOfPath(orderedVertexIdList[orderedVertexIdList.size() - 1]);
            // if there is no edge in shortest path (not possible)
            if (highestWeightedEdgeIndexOfPath == -1) {
                std::cout << "No edge in shortest path" << std::endl;
                return 0;
            }
            // mask highest weighted edge on the last found path
            edgeList[highestWeightedEdgeIndexOfPath].masked = true;
            // find the shortest path in this situation
            // if a shortest path could be found
            if ( ShortestPath(shortestPathVerticesList, from, to) ) {
                // increase the number of founded paths since we have just found one
                foundedPathsNumber++;
                // add the last found path to the ordered vertex ıd list
                orderedVertexIdList.push_back(shortestPathVerticesList);
                // clear shortest path vertices list for future use
                // !!!!!!!!!!! pass by reference ile ordered vertex ıd list'e geçiyorsa sorun !!!!!!!!!!!
                shortestPathVerticesList.clear();
            }
            // if there is no shortest path founded
            else {
                // terminate the process, and return the number of founded paths
                return foundedPathsNumber;
            }
        }
        // we could have found shortest paths in the given number of shortest paths
        // unmask all edges
        UnMaskAllEdges();
        // return the number of founded shortest paths
        return numberOfShortestPaths;
    }
    // if shortest path does not exist
    else {
        // terminate the process and return 0
        return 0;
    }
}

void Graph::MaskEdges(const std::vector<StringPair>& vertexNames) {
    // TODO
    for (int j = 0; j < vertexNames.size(); j++) {
        int vertexName0Index;
        int vertexName1Index;
        bool vertexName0Exist = false;
        bool vertexName1Exist = false;
        // if names are same
        if (vertexNames[j].s0 == vertexNames[j].s1) {
            // terminate the process for this stringPair
            continue;
        }
    
        // find the vertex with the vertexName[j]0 and vertexName[j]1 in the vertexList by traversing the whole list
        for (int i = 0; i < vertexList.size(); i++) {
            // if we have found the vertex with the vertexNames[j].s0
            if (vertexList[i].name == vertexNames[j].s0) {
                // assign the index
                vertexName0Index = i;
                // update the existence info 
                vertexName0Exist = true;
            }
            // if we have found the vertex with the vertexNames[j].s1
            else if (vertexList[i].name == vertexNames[j].s1) {
                // assign the index
                vertexName1Index = i;
                // update the existence info 
                vertexName1Exist = true;
            }
            // if this is not a vertex including given names
            else {
                // continue searching
                continue;
            }
        }
        
        // if at least one of the vertices with the given name does not exist
        if (vertexName0Exist == false || vertexName1Exist == false) {
            // give vertex not found exception
            throw VertexNotFoundException();
            continue;
        }
    
        // traverse the whole edge list
        for (int i = 0; i < edgeList.size(); i++) {
            // if this is the edge connecting the vertices with the given names
            if (edgeList[i].vertexId0 == vertexName0Index && edgeList[i].vertexId1 == vertexName1Index || edgeList[i].vertexId0 == vertexName1Index && edgeList[i].vertexId1 == vertexName0Index) {
                // make this edge's masked information true
                edgeList[i].masked = true;
            }
            // if this is not the edge connecting the vertices with the given names
            else {
                // continue searching
                continue;
            }
        }
    }
}

void Graph::UnMaskEdges(const std::vector<StringPair>& vertexNames) {
    // TODO
    for (int j = 0; j < vertexNames.size(); j++) {
        int vertexName0Index;
        int vertexName1Index;
        bool vertexName0Exist = false;
        bool vertexName1Exist = false;
        // if names are same
        if (vertexNames[j].s0 == vertexNames[j].s1) {
            // terminate the process for this stringPair
            continue;
        }
    
        // find the vertex with the vertexName[j]0 and vertexName[j]1 in the vertexList by traversing the whole list
        for (int i = 0; i < vertexList.size(); i++) {
            // if we have found the vertex with the vertexNames[j].s0
            if (vertexList[i].name == vertexNames[j].s0) {
                // assign the index
                vertexName0Index = i;
                // update the existence info 
                vertexName0Exist = true;
            }
            // if we have found the vertex with the vertexNames[j].s1
            else if (vertexList[i].name == vertexNames[j].s1) {
                // assign the index
                vertexName1Index = i;
                // update the existence info 
                vertexName1Exist = true;
            }
            // if this is not a vertex including given names
            else {
                // continue searching
                continue;
            }
        }
        
        // if at least one of the vertices with the given name does not exist
        if (vertexName0Exist == false || vertexName1Exist == false) {
            // give vertex not found exception
            throw VertexNotFoundException();
            continue;
        }
    
        // traverse the whole edge list
        for (int i = 0; i < edgeList.size(); i++) {
            // if this is the edge connecting the vertices with the given names
            if (edgeList[i].vertexId0 == vertexName0Index && edgeList[i].vertexId1 == vertexName1Index || edgeList[i].vertexId0 == vertexName1Index && edgeList[i].vertexId1 == vertexName0Index) {
                // make this edge's masked information false
                edgeList[i].masked = false;
            }
            // if this is not the edge connecting the vertices with the given names
            else {
                // continue searching
                continue;
            }
        }
    }
}

void Graph::UnMaskAllEdges() {
    // TODO
    // traverse the whole edgeList
    for (int i = 0; i < edgeList.size(); i++) {
        // make the mask data false
        edgeList[i].masked = false;
    }
}

void Graph::MaskVertexEdges(const std::string& name) {
    // TODO
    int vertexIndex;
    bool vertexExist = false;
    // find the vertex with the given name in the vertex list of graph
    for (int i = 0; i < vertexList.size(); i++) {
        // if this is the vertex that we are looking for
        if (vertexList[i].name == name) {
            // save index of the vertex
            vertexIndex = i;
            // update vertex existence data
            vertexExist = true;
        }
        else {
            // continue searching
            continue;
        }
    }

    // if the given vertex does not exist
    if (vertexExist == false) {
        // give vertex not found error
        throw VertexNotFoundException();
        // terminate the process
        return;
    }

    // traverse the whole edge list
    for (int i = 0; i < edgeList.size(); i++) {
        // if it is an edge connecting the given vertex
        if (edgeList[i].vertexId0 == vertexIndex || edgeList[i].vertexId1 == vertexIndex) {
            // unmask the edge
            edgeList[i].masked = true;
        }
        // if it is not an edge connecting the given vertex
        else {
            continue;
        }
    }
}

void Graph::UnMaskVertexEdges(const std::string& name) {
    // TODO
    int vertexIndex;
    bool vertexExist = false;
    // find the vertex with the given name in the vertex list of graph
    for (int i = 0; i < vertexList.size(); i++) {
        // if this is the vertex that we are looking for
        if (vertexList[i].name == name) {
            // save index of the vertex
            vertexIndex = i;
            // update vertex existence data
            vertexExist = true;
        }
        else {
            // continue searching
            continue;
        }
    }

    // if the given vertex does not exist
    if (vertexExist == false) {
        // give vertex not found error
        throw VertexNotFoundException();
        // terminate the process
        return;
    }

    // traverse the whole edge list
    for (int i = 0; i < edgeList.size(); i++) {
        // if it is an edge connecting the given vertex
        if (edgeList[i].vertexId0 == vertexIndex || edgeList[i].vertexId1 == vertexIndex) {
            // unmask the edge
            edgeList[i].masked = false;
        }
        // if it is not an edge connecting the given vertex
        else {
            continue;
        }
    }
}

void Graph::ModifyEdge(const std::string& vName0, const std::string& vName1, float newWeight) {
    // TODO
    int vertex0Index;
    int vertex1Index;
    bool vertex0Exist = false;
    bool vertex1Exist = false;

    if (vName0 == vName1) {
        // terminate the process
        return;
    }

    
    // find the vertex with the vName0 and vName1 in the vertexList by traversing the whole list
    for (int i = 0; i < vertexList.size(); i++) {
        // if we have found the vertex with the vName0
        if (vertexList[i].name == vName0) {
            // assign the index
            vertex0Index = i;
            // update the existence info 
            vertex0Exist = true;
        }
        // if we have found the vertex with the vName1
        else if (vertexList[i].name == vName1) {
            // assign the index
            vertex1Index = i;
            // update the existence info 
            vertex1Exist = true;
        }
        // if this is not a vertex including given names
        else {
            // continue searching
            continue;
        }
    }

    // if at least one of the vertices with the given name does not exist
    if (vertex0Exist == false || vertex1Exist == false) {
        // give vertex not found exception
        throw VertexNotFoundException();
        // terminate the process
        return;
    }

    // find the edge between vertex0 and vertex1
    for (int i = 0; i < edgeList.size(); i++) {
        // if it is the vertex connecting vertex0 and vertex1
        if (edgeList[i].vertexId0 == vertex0Index && edgeList[i].vertexId1 == vertex1Index || edgeList[i].vertexId0 == vertex1Index && edgeList[i].vertexId1 == vertex0Index ) {
            // update the vertex's weight
            edgeList[i].weight = newWeight;
        }
        // if it is not a vertex connecting vertex0 and vertex1
        else {
            continue;
        }
    }

    return;
}

void Graph::ModifyEdge(int vId0, int vId1, float newWeight) {
    // TODO
    // find the edge between vertex0 and vertex1
    for (int i = 0; i < edgeList.size(); i++) {
        // if it is the vertex connecting vertex0 and vertex1
        if (edgeList[i].vertexId0 == vId0 && edgeList[i].vertexId1 == vId1 || edgeList[i].vertexId0 == vId1 && edgeList[i].vertexId1 == vId0 ) {
            // update the vertex's weight
            edgeList[i].weight = newWeight;
        }
        // if it is not a vertex connecting vertex0 and vertex1
        else {
            continue;
        }
    }

    return;
}

void Graph::PrintAll() const {
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge& edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if(edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                             << std::setw(2) << edge.weight
                             << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                 ? edge.vertexId1
                                 : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int>& orderedVertexIdList, bool sameLine) const {
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for(size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == orderedVertexIdList.size() - 1) break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if(vertexId     < static_cast<int>(vertexList.size()) &&
           nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex& fromVert = vertexList[vertexId];
            for(int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if((edgeList[eId].vertexId0 == vertexId &&
                    edgeList[eId].vertexId1 == nextVertexId)
                ||
                   (edgeList[eId].vertexId0 == nextVertexId &&
                    edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if(edgeId != INVALID_INDEX)
        {
            const GraphEdge& edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const {
    // TODO
    return vertexList.size();
}

int Graph::TotalEdgeCount() const {
    // TODO
    return edgeList.size();
}

std::string Graph::VertexName(int vertexId) const {
    // TODO
    // if there is such an id in the vertex list of the graph
    if (vertexId < vertexList.size()) {
        // return the name of the vertex at that index
        return vertexList[vertexId].name;
    }
    // if there is not such an id in the vertex list of the graph
    else {
        // empty string
        return "";
    }
}

int Graph::TotalWeightInBetween(std::vector<int>& orderedVertexIdList) {
    // TODO
    int totalWeight = 0;

    // traverse the whole ordered vertex ıd list
    for (int i = 0; i < orderedVertexIdList.size() - 1; i++) {
        // if one of the vertices pairs does not exist in the graph
        if (orderedVertexIdList[i] >= vertexList.size() || orderedVertexIdList[i + 1] >= vertexList.size()) {
            // give vertex not found error
            throw VertexNotFoundException();
        }
        // find the index (in the edge list) of the edge between two successor vertices
        int edgeIndexBetweenVertices = FindEdgeBetweenTwoVertices(orderedVertexIdList[i], orderedVertexIdList[i + 1]);
        // if there is no edge between these two vertices
        if (edgeIndexBetweenVertices == -1) {
            // terminate the process
            return -1;
        }
        // if there is an edge between these two vertices
        else {
            // add its weight to totalWeight
            totalWeight += edgeList[edgeIndexBetweenVertices].weight;
        }
    }

    // return the total weight
    return totalWeight;
}