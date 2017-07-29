#ifndef RTT_H
#define RTT_H

#include <osg/Group>

#include <osg/StateSet>

#include <osgDB/ReadFile>

#include <osg/Camera>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>

#include <osg/PolygonMode>

/*Render To Texture*/
class RTT
{
public:
	RTT(osg::Camera::BufferComponent buffer, osg::Vec4 color);

	osg::Camera* getCamera() { return _camera.get(); }
	osg::Texture2D* getTexture() { return _texture.get(); }

private:
	void createRTTCamera(osg::Camera::BufferComponent buffer, osg::Vec4 color);

private:
	osg::ref_ptr<osg::Camera> _camera;
	osg::ref_ptr<osg::Texture2D> _texture;
};

#endif