//
//  DeliveryPlanner.cpp
//  proj4
//
//  Created by Janice Tsai on 3/7/20.
//  Copyright © 2020 Janice Tsai. All rights reserved.
//
#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
private:
    const StreetMap* m;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    totalDistanceTravelled = 0; // initialize to 0
    // for now, don't optimize vector of Delivery Requests just use given vector
    PointToPointRouter p(m);
    list<StreetSegment> deliveryRoute; // list of street segments for delivery route
    list<StreetSegment> backToDepot; // list of street segments for going back to depot after delivering last item
    vector<DeliveryRequest>::const_iterator it = deliveries.begin();
    for ( ; it != deliveries.end(); it++) {
        if (it == deliveries.begin()) {
            p.generatePointToPointRoute(depot, it->location, deliveryRoute, totalDistanceTravelled);
            // generate commands
            list<StreetSegment>::iterator it1 = deliveryRoute.begin();
            for ( ; it1 != deliveryRoute.end(); it1++) {
                
            }
        } else {
            p.generatePointToPointRoute(deliveryRoute.back().end, it->location, deliveryRoute, totalDistanceTravelled);
        }
    }
    p.generatePointToPointRoute(deliveries.back().location, depot, backToDepot, totalDistanceTravelled); // somehow figure out how to get coord of end point of last delivery
    
    return NO_ROUTE;  // Delete this line and implement this function correctly
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}

