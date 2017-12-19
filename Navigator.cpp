#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <iostream>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	AttractionMapper am;
	SegmentMapper sm;
	void NavSegmenting(NavNode& ending, vector<NavSegment>& directions) const;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	if (!ml.load(mapFile)) //if the filename is invalid
		return false;
	am.init(ml); //initialize your AttractionMapper
	sm.init(ml); //initialize your SegmentMapper
	return true;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startC, endC;
	if (!am.getGeoCoord(start, startC)) //if your start location is invalid
		return NAV_BAD_SOURCE;
	if (!am.getGeoCoord(end, endC)) //if your destination is invalid
		return NAV_BAD_DESTINATION;
	priority_queue<NavNode> open; //open list for A*
	MyMap<GeoCoord, NavNode> closed; //closed list for A*
	StreetSegment startSeg, endSeg;
	bool breaker = true;
	for (size_t i = 0; i < sm.getSegments(startC).size() && breaker; i++)
	{
		for (size_t j = 0; j < sm.getSegments(startC)[i].attractions.size(); j++)
		{
			if (lowerCase(sm.getSegments(startC)[i].attractions[j].name) == lowerCase(start))
			{
				startSeg = sm.getSegments(startC)[i]; //sets startSeg equal to the StreetSegment on which the attraction is found
				breaker = false;
				break;
			}
		}
	}
	breaker = true;
	
	for (size_t i = 0; i < sm.getSegments(endC).size() && breaker; i++)
	{
		for (size_t j = 0; j < sm.getSegments(endC)[i].attractions.size(); j++)
			if (lowerCase(sm.getSegments(endC)[i].attractions[j].name) == lowerCase(end))
			{
				endSeg = sm.getSegments(endC)[i]; //sets endSeg equal to the StreetSegment on which the attraction is found
				breaker = false;
				break;
			}
	}
	if (startSeg == endSeg)
	{
		GeoSegment user(startC, endC);
		NavSegment proceed(directionOfLine(user), endSeg.streetName, distanceEarthMiles(startC, endC), user);
		directions.clear();
		directions.push_back(proceed);
		return NAV_SUCCESS;
	}
	NavNode current(nullptr, startSeg, startC, endC);
	open.push(current); //creates a node correlating to our starting location and pushes it onto our open list
	while (!open.empty())
	{
		current = open.top(); //sets current equal to the item from the open list with the smallest f value
		open.pop(); //removes current from the open list
		if (closed.find(current.loc) == nullptr) //if this location has not already been examined
		{
			closed.associate(current.loc, current); //put our current location into the closed list
			NavNode* curPoint = closed.find(current.loc); //retrieve a pointer to our current object to be used as the parent for subsequent nodes
			vector<StreetSegment> cleaner = sm.getSegments(current.loc); //retrieves the StreetSegments associated with our current location
			for (size_t i = 0; i < cleaner.size(); i++)
			{
				if (cleaner[i] == endSeg) //if we are on the ending segment
				{
					NavNode finale(curPoint, cleaner[i], endC, endC); //go straight to the destination
					NavSegmenting(finale, directions); //fill the directions vector
					return NAV_SUCCESS; //return
				}
				if (cleaner[i].segment.start != current.loc && closed.find(cleaner[i].segment.start) == nullptr)
					open.push(NavNode(curPoint, cleaner[i],
						cleaner[i].segment.start, endC)); //if the start point of an associated segment is unsearched then push it onto the open list
				if (cleaner[i].segment.end != current.loc && closed.find(cleaner[i].segment.end) == nullptr)
					open.push(NavNode(curPoint, cleaner[i],
						cleaner[i].segment.end, endC)); //if the end point of an associated segment is unsearched then push it onto the open list
			}
		}
	}
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

void NavigatorImpl::NavSegmenting(NavNode& ending, vector<NavSegment>& directions) const
{

	stack<NavSegment> intermediate;
	while (ending.parent != nullptr)
	{
		GeoSegment user(ending.parent->loc, ending.loc);
		NavSegment proceed(directionOfLine(user), ending.strseg.streetName, ending.g - ending.parent->g, user); //push a Proceed NavSegment onto our intermediate stack
		intermediate.push(proceed);
		if (ending.parent->strseg.streetName != ending.strseg.streetName && ending.parent != nullptr) //if the streetname changed then push a Turn NavSegment onto our intermediate stack
		{
			NavSegment turn(directionOfTurn(GeoSegment((ending.parent)->parent->loc, ending.parent->loc), user), ending.strseg.streetName);
			intermediate.push(turn);
		}
		ending = *ending.parent; //set the NavNode being examined to that of the parent
	}
	directions.clear(); //empty the directions vector
	while (!intermediate.empty()) //put all the NavSegments into our directions vector in the proper order
	{
		directions.push_back(intermediate.top());
		intermediate.pop();
	}
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}