//
//  main.cpp
//  proj4
//
//  Created by Janice Tsai on 3/4/20.
//  Copyright © 2020 Janice Tsai. All rights reserved.
//
/*
#include "ExpandableHashMap.h"

#include <iostream>
using namespace std;

int main() {
    ExpandableHashMap<string,double> nameToGPA(0.3);
    nameToGPA.associate("Carey", 3.5);
    nameToGPA.associate("David", 3.99);
    nameToGPA.associate("Abe", 3.2);
    
    double* davidsGPA = nameToGPA.find("David");
    if (davidsGPA != nullptr) {
        *davidsGPA = 1.5; // works
    }
    
    nameToGPA.associate("Carey", 4.0);
    double* lindasGPA = nameToGPA.find("Carey");
    if (lindasGPA == nullptr) {
        cout << "Linda is not in roster." << endl;
    } else {
        cout << "Carey's GPA is: " << *lindasGPA << endl;
    }
}*/

#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char *argv[])
{
    /*if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }*/
    StreetMap sm;
   // if (!sm.load(argv[1]))
    if (!sm.load("/Users/janicetsai/Desktop/cs32/proj4/proj4/mapdata.txt"))
    {
        //cout << "Unable to load map data file " << argv[1] << endl;
        cout << "Unable to load map data file " << endl;
        return 1;
    }
    /*
    GeoCoord a("34.0453190", "-118.4966977");
    std::vector<StreetSegment> s;
    if (sm.getSegmentsThatStartWith(a, s)) {
        vector<StreetSegment>::iterator it;
        for (it = s.begin(); it != s.end(); it++) {
            cout << it->start.latitudeText << " " << it->start.longitudeText << " " << it->end.latitudeText << " " << it->end.longitudeText << endl;
        }
    }*/
    PointToPointRouter p(&sm);
    GeoCoord st("34.0453190", "-118.4966977"); //
   //GeoCoord en("34.0463922", "-118.4953255"); // works
   // GeoCoord en("34.0456127", "-118.3681137"); // doesn't work
    //GeoCoord en("34.0412061", "-118.4840866"); // doesn't work
    GeoCoord en("34.0359966", "-118.4907409"); // won't work
    list<StreetSegment> route;
    double totalDistTravelled = 0;
    p.generatePointToPointRoute(st, en, route, totalDistTravelled);
    
    std::list<StreetSegment>::iterator it3 = route.begin();
    for ( ; it3 != route.end(); it3++) {
        cout << it3->start.latitudeText << " " << it3->start.longitudeText << " " << it3->end.latitudeText << " " << it3->end.longitudeText << " " << it3->name << endl;
    }
    cout << "Total Distance Travelled: " << totalDistTravelled << endl;
    return 0;
    
    // don't use stuff below!
    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}

