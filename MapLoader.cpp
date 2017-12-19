#include "provided.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> mapData;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
		return false;
	string line = "";
	int numAttract = 0, attractCounter = 0;
	bool setName = false, setCoord = false;
	StreetSegment a;
	while (getline(infile, line)) //while end of file has not been reached
	{
		if (!setName) //if name has not been set
		{
			a.streetName = line; //enter the street name into a
			setName = true; //mark that the streetname has been read
		}
		else if (!setCoord) //if the GeoSegment has not been processed
		{
			GeoCoord start(line.substr(0, line.find(",")),
				line.substr(line.find_first_not_of(", ", line.find(",")), line.find(" ", line.find_first_not_of(", ", line.find(","))) - line.find_first_not_of(", ", line.find(","))));
			GeoCoord end(line.substr(line.find_first_not_of(" ", line.find(" ", line.find_first_not_of(", ", line.find(",")))),
				line.find(",", line.find_first_not_of(" ", line.find(" ", line.find_first_not_of(", ", line.find(","))))) - line.find_first_not_of(" ", line.find(" ", line.find_first_not_of(", ", line.find(","))))),
				line.substr(line.find_first_not_of(", ", line.find(",", line.find_first_not_of(", ", line.find(",")))), line.length()));
			GeoSegment connect(start, end); 
			a.segment = connect; //set a.segment equal to the GeoSegment formed by our processed data of the coordinates 
			setCoord = true; //mark that the GeoSegment has been entered
		}
		else if (attractCounter < numAttract) //if there are attractions for that StreetSegment yet to be addressed
		{
			Attraction b;
			b.name = line.substr(0, line.find("|")); //processes the atrraction name
			GeoCoord attractCoord(line.substr(line.find_first_not_of(", ", line.find("|") + 1), line.substr(0, line.find(",", line.find_first_not_of(", ", line.find("|") + 1))).length() - line.substr(0, line.find_first_not_of(", ", line.find("|") + 1)).length()),
				line.substr(line.find_first_not_of(", ", line.find(",", line.find("|"))), line.length())); //processes the coordinates of the attraction
			b.geocoordinates = attractCoord; 
			a.attractions.push_back(b); //push this attraction onto the StreetSegment's attraction vector
			++attractCounter; //mark that this attraction has been processed
			if (numAttract == attractCounter) //if all attractions for this segment were addressed
			{									//prepare for new StreetSegment on next iteration
				setName = false;
				setCoord = false;
				mapData.push_back(a);
				a.attractions.clear();
			}
		}
		else if (setCoord && setName) //if both the name and GeoSegment of the StreetSegment have been addressed then see if there are possible attractions to process
		{
			numAttract = stoi(line);
			attractCounter = 0;
			if (numAttract == attractCounter) //if no attractions, prepare for new StreetSegment
			{
				setName = false;
				setCoord = false;
				mapData.push_back(a);
				a.attractions.clear();
			}
		}
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return mapData.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= mapData.size()) //returns false if invalid index
		return false;
	seg = mapData[segNum]; 
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
