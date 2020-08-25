//
//  StreetMap.cpp
//  proj4
//
//  Created by Janice Tsai on 3/5/20.
//  Copyright © 2020 Janice Tsai. All rights reserved.
//
#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> myHash;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if (!infile) {
        return false; // loading file does not work so return false
    }
    string s;
    while(true) {
        if (!getline(infile, s)) {
            return true;
        }
        string streetName = s;
        getline(infile, s);
        int count = stoi(s);
        for (int i = 0; i < count; i++) {
            getline(infile, s);
            stringstream ss(s);
            string a, b, c, d;
            ss >> a >> b >> c >> d;
            GeoCoord start(a, b);
            GeoCoord end(c, d);
            StreetSegment streetseg1(start, end, streetName);
            vector<StreetSegment>* v1 = myHash.find(start);
            if (v1 == nullptr) { // not found in map
                vector<StreetSegment> v;
                myHash.associate(start, v);
                v.push_back(streetseg1);
            } else {
                v1->push_back(streetseg1);
            }
            StreetSegment streetseg2(end, start, streetName);
            vector<StreetSegment>* v2 = myHash.find(end);
            if (v2 == nullptr) { // not found in map
                vector<StreetSegment> v;
                myHash.associate(end, v);
                v.push_back(streetseg2);
            } else {
                v2->push_back(streetseg2);
            }
        }
    }
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    const vector<StreetSegment>* v = myHash.find(gc);
    if (v != nullptr) { // found gc starting point in myHash
        segs.clear();
        segs = *v;
        return true;
    }
    return false;
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
