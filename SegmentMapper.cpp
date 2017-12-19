#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> mappedCoords;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment temp;
	vector<StreetSegment> tempVec;
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		ml.getSegment(i, temp); //for each segment
		if (mappedCoords.find(temp.segment.start)) //if this GeoCoord is already in the MyMap
			mappedCoords.find(temp.segment.start)->push_back(temp); //push the corresponding StreetSegment into the vector for that GeoCoord
		else //else put this GeoCoord into the MyMap and push the StreetSegment into it's vector
		{
			mappedCoords.associate(temp.segment.start, tempVec);
			mappedCoords.find(temp.segment.start)->push_back(temp);
		}
		if (mappedCoords.find(temp.segment.end)) //if this GeoCoord is already in the MyMap
			mappedCoords.find(temp.segment.end)->push_back(temp); //push the corresponding StreetSegment into the vector for that GeoCoord
		else //else put this GeoCoord into the MyMap and push the StreetSegment into it's vector
		{
			mappedCoords.associate(temp.segment.end, tempVec);
			mappedCoords.find(temp.segment.end)->push_back(temp);
		}
		for (size_t attractNum = 0; attractNum < temp.attractions.size(); attractNum++)
		{
			if (mappedCoords.find(temp.attractions[attractNum].geocoordinates)) // if this GeoCoord is already in the MyMap
				mappedCoords.find(temp.attractions[attractNum].geocoordinates)->push_back(temp); //push the corresponding StreetSegment into the vector for that GeoCoord
			else
			{ //else put this GeoCoord into the MyMap and push the StreetSegment into it's vector
				if (temp.attractions[attractNum].geocoordinates != temp.segment.start && temp.attractions[attractNum].geocoordinates != temp.segment.end) //if the attraction is not on the end 
				{																																		//of a segment
					mappedCoords.associate(temp.attractions[attractNum].geocoordinates, tempVec); 
					mappedCoords.find(temp.attractions[attractNum].geocoordinates)->push_back(temp);
				}
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	return *(mappedCoords.find(gc)); //return the return value of the MyMap::find function
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
