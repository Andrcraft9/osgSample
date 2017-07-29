#ifndef DOOMMANIPULATOR_H
#define DOOMMANIPULATOR_H

#include <iostream>
#include <osg/Group>
#include <osg/Geometry>
#include <osg/PolygonMode>

#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osg/Program>
#include <osgGA/SphericalManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/DriveManipulator>


class DoomManipulator : public osgGA::FirstPersonManipulator
{
public:
	DoomManipulator() {}
	void moveForwardLinear(double distance);
	void moveRightLinear(double distance);
	void moveLeftLinear(double distance);
	
private:

	bool handleMouseWheel(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
	bool handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us);
};

#endif