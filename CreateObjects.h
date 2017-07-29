#ifndef CREATEOBHECTS_H
#define CREATEOBJECTS_H

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

class CreateObjects
{
public:
	static osg::Geode* createSphere(osg::Vec3d center, double radius);
	static osg::Node* createSquare(osg::Vec3d point, double size);
	static osg::Node* createMeshSquare(osg::Vec3d point, double size, int lvl);
	static osg::Node* createMeshPlane(osg::Vec3d point, double size, int lvl);
	static osg::Node* createBox(osg::Vec3d center, double lenX, double lenY, double lenZ);
};

#endif