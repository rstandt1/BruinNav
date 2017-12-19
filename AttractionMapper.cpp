#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> mappedAttractions;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment temp;
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		ml.getSegment(i, temp); //for each StreetSegment
		for (size_t attractNum = 0; attractNum < temp.attractions.size(); attractNum++)
		{
			mappedAttractions.associate(lowerCase(temp.attractions[attractNum].name), temp.attractions[attractNum].geocoordinates); //associate each attraction's coordinates with it's name
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	if (mappedAttractions.find(lowerCase(attraction)) == nullptr) //returns false if the attraction name is not found in mappedAttractions
		return false;
	gc = *(mappedAttractions.find(lowerCase(attraction))); //set gc equal to the dereferenced pointer returned by the function call mappedAttractions.find
	return true;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
