#include "DoomManipulator.h"


bool DoomManipulator::handleMouseWheel(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	return false;
}

bool DoomManipulator::handleKeyDown(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	switch (ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::KEYDOWN) :
		{
			switch (ea.getKey())
			{
			case 'w':
				moveForwardLinear(10);
				return true;
			case 'a':
				moveLeftLinear(10);
				return true;
			case 'd':
				moveRightLinear(10);
				return true;
			case 's':
				moveForwardLinear(-10);
				return true;
			default:
				return false;
			}
		}
	}
}

void DoomManipulator::moveForwardLinear(double distance)
{
	osg::Vec3d eye;
	eye = _eye + _rotation * osg::Vec3d(0., 0., -distance);
	if (eye.z() < 200 && eye.z() > -150)
		_eye = eye;

}

/// Move camera right by distance parameter.
void DoomManipulator::moveRightLinear(double distance)
{
	_eye += _rotation * osg::Vec3d(distance, 0., 0.);
}


/// Move camera up by distance parameter.
void DoomManipulator::moveLeftLinear(double distance)
{
	_eye += _rotation * osg::Vec3d(-distance, 0., 0.);
}
/*
bool DoomManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
	switch (ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::KEYDOWN) :
		{
			switch (ea.getKey())
			{
			case 'w':
				moveForward(100);
				return true;
				break;
			default:
				return false;
			}
		}
		case(osgGA::GUIEventAdapter::DRAG) :
		{
			//handleMouseDrag(ea, us);
			handleMouseDeltaMovement(ea, us);


			return true;
		}
		default:
			return false;
	}
	if (isMouseMoving())
	{
		addMouseEvent(ea);
		us.requestContinuousUpdate(true);
		if (performMovement())
			us.requestRedraw();

		return true;
	}
}
*/