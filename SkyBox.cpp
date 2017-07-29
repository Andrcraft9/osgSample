#include "SkyBox.h"

#include "CreateObjects.h"

void SkyBox::createTextureCubeMap()
{
	// Create TextureCubeMap
	osg::TextureCubeMap *texture_cube_map = new osg::TextureCubeMap;
	texture_cube_map->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_EDGE);
	texture_cube_map->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_EDGE);
	texture_cube_map->setWrap(osg::Texture::WRAP_R, osg::Texture::WrapMode::CLAMP_TO_EDGE);
	texture_cube_map->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture_cube_map->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	
	osg::ref_ptr<osg::Image> ft = osgDB::readImageFile("textures/cubemap/saturn_cube_map/ft.tga");
	osg::ref_ptr<osg::Image> bk = osgDB::readImageFile("textures/cubemap/saturn_cube_map/bk.tga");
	osg::ref_ptr<osg::Image> lf = osgDB::readImageFile("textures/cubemap/saturn_cube_map/rt.tga");
	osg::ref_ptr<osg::Image> rt = osgDB::readImageFile("textures/cubemap/saturn_cube_map/lf.tga");
	osg::ref_ptr<osg::Image> up = osgDB::readImageFile("textures/cubemap/saturn_cube_map/up.tga");
	osg::ref_ptr<osg::Image> dn = osgDB::readImageFile("textures/cubemap/saturn_cube_map/dn.tga");
	
	texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_X, ft.get());
	texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_X, bk.get());
	texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Y, lf.get());
	texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Y, rt.get());
	texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Z, up.get());
	texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Z, dn.get());
	
	_texture_cube_map = texture_cube_map;

	return;
}

void SkyBox::createSkyBox()
{
	osg::ref_ptr<osg::Node> skyBox = CreateObjects::createBox(osg::Vec3d(0, 0, 0), 1, 1, 1);
	osg::StateSet* stateset = skyBox->getOrCreateStateSet();

	// Set texture
	stateset->setTextureAttributeAndModes(0, _texture_cube_map.get(), osg::StateAttribute::ON);
	// Set shader
	osg::ref_ptr<osg::Program> program = new osg::Program;
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/skybox.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/skybox.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	//stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// Set size
	osg::ref_ptr<osg::MatrixTransform> rootSkyBox = new osg::MatrixTransform();
	rootSkyBox->setMatrix(osg::Matrix::scale(osg::Vec3d(_scale, _scale, _scale)));
	rootSkyBox->addChild(skyBox.get());

	_skyBox = rootSkyBox;
	
	return;
}
