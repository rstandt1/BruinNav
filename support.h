#ifndef SUPPORT
#define SUPPORT

#include "provided.h"
#include<string>
struct NavNode
{
	NavNode()
	{}
	NavNode(NavNode* par, StreetSegment seg, GeoCoord& spot, GeoCoord& end)  //constructor to initialize new NavNodes
		: parent(par), strseg(seg), loc(spot)
	{
		if (parent == nullptr) //if initializing our starting NavNode
			g = 0;
		else
			g = parent->g + distanceEarthMiles(parent->loc, loc); //sets our g value equal to that of the parent plus the distance to our new position
		h = distanceEarthMiles(spot, end); //distance from our new position to the destination
		f = g + h;
	}
	NavNode* parent;
	StreetSegment strseg;
	GeoCoord loc;
	double f, g, h;
};
bool operator==(const GeoCoord& coord1, const GeoCoord& coord2);
bool operator==(const StreetSegment& seg1, const StreetSegment& seg2);
bool operator!=(const GeoCoord& coord1, const GeoCoord& coord2);
bool operator<(const GeoCoord& coord1, const GeoCoord& coord2);
bool operator>(const GeoCoord& coord1, const GeoCoord& coord2);
bool operator<(const NavNode& node1, const NavNode& node2);
std::string lowerCase(std::string name);
std::string directionOfLine(const GeoSegment& seg);
std::string directionOfTurn(const GeoSegment& seg1, const GeoSegment& seg2);
#endif // !SUPPORT

