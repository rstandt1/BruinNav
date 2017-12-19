#include "support.h"
#include<string>
using namespace std;

bool operator==(const GeoCoord& coord1, const GeoCoord& coord2)
{
	if (coord1.latitude == coord2.latitude && coord1.longitude == coord2.longitude) //returns true if the longitude and latitude coordinates of both GeoCoord's are equal
		return true;
	else
		return false;
}

bool operator==(const StreetSegment& seg1, const StreetSegment& seg2)
{
	bool a = (seg1.streetName == seg2.streetName);
	bool b = (seg1.segment.start == seg2.segment.start);
	bool c = (seg1.segment.end == seg2.segment.end);
	return (a && b && c); //returns true if the streetnames and GeoSegments are identical
}

bool operator!=(const GeoCoord& coord1, const GeoCoord& coord2)
{
	return !(coord1 == coord2); //returns the opposite of our overloaded == operator
}

bool operator<(const NavNode& node1, const NavNode& node2)
{
	return (node1.f > node2.f); //used in priority queue in Navigator to ensure that NavNode with smallest f value is the next one popped off the queue
}

bool operator<(const GeoCoord& coord1, const GeoCoord& coord2)
{
	if (coord1.latitude < coord2.latitude) //if coord1 has a lesser latitude then return true
		return true;
	else if (coord1.latitude > coord2.latitude) //if coord2 has a lesser latitude then return false
		return false;
	else //if the latitudes are the same
	{
		if (coord1.longitude < coord2.longitude) //return true if coord1's longitude is less than coord2's
			return true;
		else
			return false; //return false otherwise
	}
}

bool operator>(const GeoCoord& coord1, const GeoCoord& coord2)
{
	if (coord1.latitude > coord2.latitude) //if coord2 has a lesser latitude then return true
		return true;
	else if (coord1.latitude < coord2.latitude) //if coord1 has a lesser latitude then return false
		return false;
	else //if the latitudes are the same
	{
		if (coord1.longitude > coord2.longitude) //return true if coord2's longitude is less than coord2's
			return true;
		else
			return false; //return false otherwise
	}
}

string lowerCase(string name)
{
	for (size_t i = 0; i < name.length(); i++)
		name[i] = tolower(name[i]);
	return name; //returns a lowercase version of name
}

string directionOfLine(const GeoSegment& gs)
{
	double angle = angleOfLine(gs); //returns the direction a GeoSegment goes in as specified by the spec
	if (angle >= 0 && angle <= 22.5)
		return "east";
	if (angle > 22.5 && angle <= 67.5)
		return "northeast";
	if (angle > 67.5 && angle <= 112.5)
		return "north";
	if (angle > 112.5 && angle <= 157.5)
		return "northwest";
	if (angle > 157.5 && angle <= 202.5)
		return "west";
	if (angle > 202.5 && angle <= 247.5)
		return "southwest";
	if (angle > 247.5 && angle <= 292.5)
		return "south";
	if (angle > 292.5 && angle <= 337.5)
		return "southeast";
	if (angle > 337.5 && angle <= 360)
		return "east";
}

string directionOfTurn(const GeoSegment& seg1, const GeoSegment& seg2)
{
	double angle = angleBetween2Lines(seg1, seg2); // calculates the direction of a turn as specified by the spec
	if (angle >= 0 && angle < 180)
		return "left";
	if (angle >= 180)
		return "right";
}
