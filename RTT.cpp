#include "RTT.h"

RTT::RTT(osg::Camera::BufferComponent buffer, osg::Vec4 color)
{
	_camera = new osg::Camera;

	_texture = new osg::Texture2D;
	_texture->setTextureSize(1024, 1024);
	_texture->setInternalFormat(GL_RGBA);

	//_texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	//_texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	//_texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	//_texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

	createRTTCamera(buffer, color);
}

void RTT::createRTTCamera(osg::Camera::BufferComponent buffer, osg::Vec4 color)
{
	_camera->setClearColor(color);
	_camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	_camera->setRenderOrder(osg::Camera::PRE_RENDER);

	if (_texture)
	{
		_texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
		_texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
		_camera->setViewport(0, 0, _texture->getTextureWidth(), _texture->getTextureHeight());
		_camera->attach(buffer, _texture);

	}

	return;
}
