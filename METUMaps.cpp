#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const {
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const {
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const {
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const {
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const {
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const {
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const {
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string& location0, const std::string& location1) const {
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string& location0, const std::string& location1) const {
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string& location0, const std::string& location1) {
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount, const std::string& mapFilePath) {
    // TODO
    // create the graph pf maps
    Graph mapGraph(mapFilePath);
    // assign this graph to our map
    map = mapGraph;
    // set potential map count
    this->potentialPathCount = potentialPathCount;
    // initially class is not in journey
    inJourney = false;
    // create a KeyedHashTable with the size of total location count in the map times given potential path count
    KeyedHashTable mapTable(map.TotalVertexCount() * potentialPathCount);
    // assign this hash table to cached paths
    cachedPaths = mapTable;
}

void METUMaps::SetDestination(const std::string& name) {
    // TODO
    // NOT TESTED
    // if the user in journey
    if (inJourney == true) {
        PrintUnableToChangeDestination();
    }
    else {
        // set destination to the given name
        destination = name;
    }
}

void METUMaps::SetStartingLocation(const std::string& name) {
    // TODO
    // NOT TESTED
    // if the user in journey
    if (inJourney == true) {
        PrintUnableToChangeDestination();
    }
    else {
        // set starting location to the given name
        startingLoc = name;
    }
}

void METUMaps::StartJourney() {
    // TODO

    // give a message to user
    PrintCalculatingRoutes(startingLoc, destination);

    // if the user is in journey
    if (inJourney) {
        PrintAlreadyInJourney();
        return;
    }
    // if startingLoc or destination is not in the map
    try {
        std::vector<int> shortestPathList;
        // if Shortest path gives VertexNotFoundException
        map.ShortestPath(shortestPathList, startingLoc, destination);
    }
    catch(VertexNotFoundException& e) {
        // print error
        PrintLocationNotFound();
        // terminate the process
        return;
    }

    // create a vector to put short paths
    std::vector< std::vector<int> > multipleShortPathsList;
    // find short paths in the number of potentialPathCount
    map.MultipleShortPaths(multipleShortPathsList, startingLoc, destination, potentialPathCount);

    // create a std::vector to put all sub paths
    std::vector< std::vector<int> > subPaths;
    // cache founded short paths
    // traverse the whole multiple short paths list
    for (int i = 0; i < multipleShortPathsList.size(); i++) {
        // traverse the elements of a short list in the multiple short paths list
        for (int k = 0; k < multipleShortPathsList[i].size() - 1; k++) {
            // create a vector to put a sub path
            std::vector<int> subPath;
            for (int j = k; j < multipleShortPathsList[i].size(); j++) {
                subPath.push_back(multipleShortPathsList[i][j]);
            }
            // append it to subPaths
            subPaths.push_back(subPath);
        }
    }

    // traverse the whole subPaths
    for (int i = 0; i < subPaths.size(); i++) {
        // insert it to the cached paths hash table
        cachedPaths.Insert(GenerateKey(map.VertexName(subPaths[i][0]), map.VertexName(subPaths[i][subPaths[i].size() - 1])), subPaths[i]);
        // ------------------Test-------------------------
        /*
        std::cout << "Key: " << GenerateKey(map.VertexName(subPaths[i][0]), map.VertexName(subPaths[i][subPaths[i].size() - 1])) << std::endl;
        std::cout << "Sub Path: ";
        for (int a = 0; a < subPaths[i].size(); a++) {
            std::cout << map.VertexName(subPaths[i][a]) << " ";
        }
        std::cout << "\n";
        */
        // ------------------Test-------------------------

    }

    inJourney = true;
    currentLoc = startingLoc;
    currentRoute = multipleShortPathsList[0];
}

void METUMaps::EndJourney() {
    // TODO
    // if the user is not in a journey
    if (!inJourney) {
        PrintJourneyIsAlreadFinished();
        return;
    }

    // clear cached paths
    cachedPaths.ClearTable();

    // clear startingLoc, currentLoc, destination
    startingLoc = "";
    currentLoc = "";
    destination = "";
    return;
}

void METUMaps::UpdateLocation(const std::string& name) {
    // TODO
    if (!inJourney) {
        // print an error message
        PrintNotInJourney();
        // terminate the process
        return;
    }

    // if name is not in the map
    try {
        // create a vector to put shortest path
        std::vector<int> shortestPathTry;
        // try to find shortest path from name to destination
        map.ShortestPath(shortestPathTry, name, destination);
    }
    // if Shortest path gives VertexNotFoundException
    catch(VertexNotFoundException& e) {
        // print an error message
        PrintLocationNotFound();
        // terminate the process
        return;
    }

    // if name is equal to destination
    if (name == destination) {
        // print an error message
        PrintJourneyCompleted();
        // terminate the process
        return;
    }

    // create a new vector to put a cached new route from name to destination
    std::vector<int> newRoute;
    // find a new route from name to destination in cached paths
    bool isCached = cachedPaths.Find(newRoute, GenerateKey(name, destination));
    // if such a route hes been found in the cached paths
    if (isCached) {
        // print a message
        PrintCachedLocationFound(name, destination);
        // update current location
        currentLoc = name; 
        // set current route to new route
        currentRoute = newRoute;
    }
    // if there is no any route from name to destination in the cached paths
    else {
        // print a message
        PrintCalculatingRoutes(name, destination);
        // create a vector to put shortest path between name and destination
        std::vector<int> shortestPath;
        // find shortest path from name to destination
        // if there is a path from name to destination
        if (map.ShortestPath(shortestPath, name, destination)) {
            // create a std::vector to put all sub paths
            std::vector< std::vector<int> > subPaths;
            // put shortest path inside multiple short paths list to be consistent
            std::vector< std::vector<int> > multipleShortPathsList;
            multipleShortPathsList.push_back(shortestPath);
            // cache founded short paths
            // traverse the whole multiple short paths list
            for (int i = 0; i < multipleShortPathsList.size(); i++) {
                // traverse the elements of a short list in the multiple short paths list
                for (int k = 0; k < multipleShortPathsList[i].size() - 1; k++) {
                    // create a vector to put a sub path
                    std::vector<int> subPath;
                    for (int j = k; j < multipleShortPathsList[i].size(); j++) {
                        subPath.push_back(multipleShortPathsList[i][j]);
                    }
                    // append it to subPaths
                    subPaths.push_back(subPath);
                }
            }
        
            // traverse the whole subPaths
            for (int i = 0; i < subPaths.size(); i++) {
                // insert it to the cached paths hash table
                cachedPaths.Insert(GenerateKey(map.VertexName(subPaths[i][0]), map.VertexName(subPaths[i][subPaths[i].size() - 1])), subPaths[i]);
            }

            currentLoc = name;
            currentRoute = shortestPath;
        }
        // if there is no any path from name to destination
        else {
            // terminate the process
            return;
        }
    }
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if(!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}