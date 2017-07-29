#ifndef SKYBOX_H
#define SKYBOX_H

#include <osg/Group>
#include <osg/Geometry>
#include <osg/PolygonMode>

#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>

#include <osgDB/ReadFile>

#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osg/Program>

class SkyBox
{
public:
	SkyBox(int scale) : _scale(scale) { createTextureCubeMap(); createSkyBox(); }

	osg::Node* getSkyBox() { return _skyBox.get(); }
	osg::TextureCubeMap* getCubeMap() { return _texture_cube_map.get(); }

private:
	void createTextureCubeMap();
	void createSkyBox();

private:
	osg::ref_ptr<osg::TextureCubeMap> _texture_cube_map;
	osg::ref_ptr<osg::Node> _skyBox;
	int _scale;
};

#endif