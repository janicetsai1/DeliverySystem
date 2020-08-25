//
//  PointToPointRouter.cpp
//  proj4
//
//  Created by Janice Tsai on 3/7/20.
//  Copyright © 2020 Janice Tsai. All rights reserved.
//
#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <vector>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
private:
    struct Node {
        double f, g, h;
        GeoCoord coord;
        StreetSegment s;
        Node* prev;
        Node(GeoCoord curr) : coord(curr) {
            f = -1;
            g = 0;
            h = 0;
        }
    };
    const StreetMap* myMap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    myMap = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    double totalTravelled = 0;
    totalDistanceTravelled = 0;
    if (start == end) {
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    vector<StreetSegment> s; // do i need these??
    vector<StreetSegment> y;
    myMap->getSegmentsThatStartWith(start, s);
    if (s.empty()) {
        return BAD_COORD;
    }
    myMap->getSegmentsThatStartWith(end, y);
    if (y.empty()) {
        return BAD_COORD;
    }
    vector<Node> openList; // everything not explored
    vector<Node> closedList; // everything already explored
    bool firstPoint = true;
    Node currVertex(start);
    currVertex.g = distanceEarthMiles(currVertex.coord, start);
    currVertex.h = distanceEarthMiles(currVertex.coord, end);
    currVertex.f = currVertex.g + currVertex.h;
    while (currVertex.coord != end) {
        GeoCoord one = currVertex.coord; // coord of current vertex (where you're moving from)
        vector<StreetSegment> possibleSegments;
        myMap->getSegmentsThatStartWith(currVertex.coord, possibleSegments); // possibleSegment vector now holds StreetSegments that start with currVertex coordinate
        for (int i = 0; i < possibleSegments.size(); i++) {
            GeoCoord x(possibleSegments[i].end); // where you want to move to
            Node p(x);
            Node* possibleVertex = &p; // Node for the coordinate you want to move to
            possibleVertex->s = possibleSegments[i]; // keeps track of the street segment associated with possible vertex
            bool inOpen = false; // assume false
            bool inClosed = false; // assume false
            vector<Node>::iterator itOpen = openList.begin();
            for ( ; itOpen != openList.end(); itOpen++) {
                if (itOpen->s == possibleVertex->s) {
                    inOpen = true;
                    break;
                }
            }
            vector<Node>::iterator itClosed = closedList.begin();
            for ( ; itClosed != closedList.end(); itClosed++) {
                if (itClosed->s == possibleVertex->s) {
                    inClosed = true;
                    break;
                }
            }
            if (!inOpen && !inClosed) {
                double currG = distanceEarthMiles(possibleVertex->coord, start);
                double currH = distanceEarthMiles(possibleVertex->coord, end);
                double currF = currG + currH;
                possibleVertex->g = currG;
                possibleVertex->h = currH;
                if (currF < possibleVertex->f || possibleVertex->f == -1) {
                    possibleVertex->f = currF;
                    possibleVertex->prev = &currVertex;
                }
                openList.push_back(*possibleVertex); // push back nodes (possible vertexes) into openList
            } else if (inOpen && !inClosed) {
                double currG = distanceEarthMiles(possibleVertex->coord, start);
                double currH = distanceEarthMiles(possibleVertex->coord, end);
                double currF = currG + currH;
                if (currF < itOpen->f) {
                    itOpen->g = currG;
                    itOpen->h = currH;
                    itOpen->f = currF;
                    itOpen->prev = currVertex;
                    itOpen->s = possibleSegments[i];
                }
            }
        }
        // by now, openList will contain nodes of vertexes you wish to move to
        if (openList.empty()) {
            // means you either don't have a route or must take different paths
            return NO_ROUTE;
            currVertex = *currVertex.prev; // somehow trace back??
        }
        if (firstPoint) { // first vertex is starting point so no need to put this into closedList
            firstPoint = false;
        } else {
            closedList.push_back(currVertex); // add vertex you are coming from to closedList because this means you've already visited
        }
        // find best vertex to move to
        vector<Node>::iterator it = openList.begin();
        int index = 0;
        int minIndex = 0;
        if (openList.size() > 0) { // double check that openList size is greater than 0
            double minF = it->f; // initialize minF to be f of first node in openList
            for ( ; it != openList.end(); it++) {
                currVertex = *it; // in case there's only one possible vertex to move to in openList
                if (it->f < minF) {
                    minF = it->f;
                    currVertex = *it;
                    minIndex = index;
                }
                index++;
            }
        }
        totalDistanceTravelled += distanceEarthMiles(one, currVertex.coord);

        for (int i = 0; i < openList.size(); i++) {
            if (i == minIndex) {
                openList.erase(openList.begin() + minIndex);
                break;
            }
        }
        cout << "hi" << endl;
    }
    closedList.push_back(currVertex);
    
    // loop through, using it, closedList nodes from beg to end and put it->s in route list
    vector<Node>::iterator it2 = closedList.begin();
    for ( ; it2 != closedList.end(); it2++) {
        route.push_back(it2->s);
    }
  //  totalDistanceTravelled = totalTravelled;
    // if no route found, return NO_ROUTE
    if (!route.empty()) {
        cout << "route good" << endl;
        return DELIVERY_SUCCESS;
    }
    return NO_ROUTE; // becaues by the time it gets here, it means that route list is empty
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
