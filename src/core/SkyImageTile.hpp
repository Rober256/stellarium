/*
 * Copyright (C) 2008 Fabien Chereau
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef _SKYIMAGETILE_HPP_
#define _SKYIMAGETILE_HPP_

#include "StelTextureTypes.hpp"
#include "SphereGeometry.hpp"
#include "MultiLevelJsonBase.hpp"

#include <QTimeLine>

//#define DEBUG_SKYIMAGE_TILE

class QIODevice;
class StelCore;
class StelPainter;

//! Contain all the credits for a given server hosting the data
class ServerCredits
{
public:
	//! Very short credit to display in the loading bar
	QString shortCredits;
	
	//! Full credits
	QString fullCredits;
	
	//! The URL where to get more info about the server
	QString infoURL;
};

//! Contains all the credits for the creator of the image collection
class DataSetCredits
{
public:
	//! Very short credit to display in the loading bar
	QString shortCredits;
	
	//! Full credits
	QString fullCredits;
	
	//! The URL where to get more info about the data collection
	QString infoURL;
};

//! Base class for any astro image with a fixed position
class SkyImageTile : public MultiLevelJsonBase
{
	Q_OBJECT;
	
	friend class SkyImageMgr;
	
public:
	//! Default constructor
	SkyImageTile() {initCtor();}
	
	//! Constructor
	SkyImageTile(const QString& url, SkyImageTile* parent=NULL);
	//! Constructor
	SkyImageTile(const QVariantMap& map, SkyImageTile* parent);
	
	//! Destructor
	~SkyImageTile();

	//! Draw the image on the screen.
	void draw(StelCore* core, const StelPainter& sPainter);
	
	//! Return the dataset credits to use in the progress bar
	DataSetCredits getDataSetCredits() const {return dataSetCredits;}
	
	//! Return the server credits to use in the progress bar
	ServerCredits getServerCredits() const {return serverCredits;}
	
	//! Return true if the tile is fully loaded and can be displayed
	bool isReadyToDisplay() const;

	//! Convert the image informations to a map following the JSON structure.
	//! It can be saved as JSON using the QtJsonParser methods.
	QVariantMap toQVariantMap() const;
	
	//! Return the absolute path/URL to the image file
	QString getAbsoluteImageURI() const {return absoluteImageURI;}

	//! Delete the texture from memory. It will be reloaded automatically if needed
	void deleteTexture() {tex.reset();}
	
protected:
	//! Minimum resolution of the data of the texture in degree/pixel
	float minResolution;
	
	//! The credits of the server where this data come from
	ServerCredits serverCredits;
	
	//! The credits for the data set
	DataSetCredits dataSetCredits;
	
	//! URL where the image is located
	QString absoluteImageURI;
	
	//! The image luminance in cd/m^2
	float luminance;
	
	//! Whether the texture must be blended
	bool alphaBlend;
	
	//! True if the tile is just a list of other tiles without texture for itself
	bool noTexture;
	
	//! Direction of the vertices of each polygons in ICRS frame
	QList<StelGeom::ConvexPolygon> skyConvexPolygons;
	
	//! Positions of the vertex of each convex polygons in texture space
	QList< QList<Vec2f> > textureCoords;

protected:

	//! Load the tile from a valid QVariantMap
	virtual void loadFromQVariantMap(const QVariantMap& map);
	
private:
	//! init the SkyImageTile
	void initCtor();
	
	//! Return the list of tiles which should be drawn.
	//! @param result a map containing resolution, pointer to the tiles
	void getTilesToDraw(QMultiMap<double, SkyImageTile*>& result, StelCore* core, const StelGeom::ConvexPolygon& viewPortPoly, float limitLuminance, bool recheckIntersect=true);
	
	//! Draw the image on the screen.
	//! @return true if the tile was actually displayed
	bool drawTile(StelCore* core, const StelPainter& sPainter);
	
	//! Return the minimum resolution
	double getMinResolution() const {return minResolution;}
	
	// The texture of the tile
	StelTextureSP tex;

	// Used for smooth fade in
	QTimeLine* texFader;
	
#ifdef DEBUG_SKYIMAGE_TILE
	static class StelFont* debugFont;
#endif
};

#endif // _SKYIMAGETILE_HPP_
