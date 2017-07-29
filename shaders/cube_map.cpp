#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <iostream>
#include <osg/TextureCubeMap>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgViewer/CompositeViewer>

#include <osg/TexMat>
#include <osg/TexGen>
#include <osg/TexEnv>

using namespace std;
//namespace cube_map
//{

	int main()
	{
		osg::ref_ptr<osgViewer::View> view = new osgViewer::View();
		osg::ref_ptr<osgGA::CameraManipulator> camera_manipulator = new osgGA::TrackballManipulator;

		view->setCameraManipulator(camera_manipulator);

		osg::Camera* camera = view->getCamera();

		osg::ref_ptr<osg::Group> root = new osg::Group;

		//------------Program------------

		osg::ref_ptr<osg::Shader> vert_shader = new osg::Shader(osg::Shader::VERTEX);
		vert_shader->loadShaderSourceFromFile("cube_map.vert");
		osg::ref_ptr<osg::Shader> frag_shader = new osg::Shader(osg::Shader::FRAGMENT);
		frag_shader->loadShaderSourceFromFile("cube_map.frag");

		osg::ref_ptr<osg::Program>  program = new osg::Program;
		program->addShader(vert_shader);
		program->addShader(frag_shader);
		program->addBindFragDataLocation("VertexPosition", 0);
		program->addBindFragDataLocation("VertexNormal", 1);
		program->addBindFragDataLocation("VertexTexCoord", 2);

		//------------Cube map------------

		// create and setup the texture object
		osg::TextureCubeMap *texture_cube_map = new osg::TextureCubeMap;
		texture_cube_map->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		texture_cube_map->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		texture_cube_map->setWrap(osg::Texture::WRAP_R, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		texture_cube_map->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
		texture_cube_map->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

		// assign the six images to the texture object
		/*
		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("Textures/starfield/pos_x.png"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("Textures/starfield/neg_x.png"));
		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("Textures/starfield/pos_y.png"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("Textures/starfield/neg_y.png"));
		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("Textures/starfield/pos_z.png"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("Textures/starfield/neg_z.png"));
		*/

		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_X, osgDB::readImageFile("Textures/Yokohama_2/posx.tga"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_X, osgDB::readImageFile("Textures/Yokohama_2/negx.tga"));
		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Y, osgDB::readImageFile("Textures/Yokohama_2/posy.tga"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Y, osgDB::readImageFile("Textures/Yokohama_2/negy.tga"));
		texture_cube_map->setImage(osg::TextureCubeMap::POSITIVE_Z, osgDB::readImageFile("Textures/Yokohama_2/posz.tga"));
		texture_cube_map->setImage(osg::TextureCubeMap::NEGATIVE_Z, osgDB::readImageFile("Textures/Yokohama_2/negz.tga"));

		

		//------------Uniforms------------

		osg::ref_ptr<osg::Node> box_0 = osgDB::readNodeFile("models/cube.obj");

		osg::StateSet* box_state_set = box_0->getOrCreateStateSet();


		//box_state_set->setAttributeAndModes(program, osg::StateAttribute::ON);

		//box_state_set->getOrCreateUniform("DrawSkyBox", osg::Uniform::Type::BOOL)->set(true);
		//box_state_set->getOrCreateUniform("ReflectFactor", osg::Uniform::Type::FLOAT)->set(1.0f);
		//box_state_set->getOrCreateUniform("MaterialColor", osg::Uniform::Type::FLOAT_VEC4)->set(osg::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));

		//box_state_set->setTextureAttributeAndModes(0, texture_cube_map, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);
		box_state_set->setTextureAttributeAndModes(0, texture_cube_map, osg::StateAttribute::ON);

		osg::ref_ptr<osg::MatrixTransform> sky_box = new osg::MatrixTransform;
		sky_box->setMatrix(osg::Matrix::scale(1000.0f, 1000.0f, 1000.0f));
		sky_box->addChild(box_0.get());

		root->addChild(sky_box.get());

		osg::ref_ptr<osg::Node> sphere_0 = osgDB::readNodeFile("models/glonass1.obj");

		osg::StateSet* sphere_state_set = sphere_0->getOrCreateStateSet();

		sphere_state_set->setAttributeAndModes(program, osg::StateAttribute::ON);

		sphere_state_set->getOrCreateUniform("DrawSkyBox", osg::Uniform::Type::BOOL)->set(false);
		sphere_state_set->getOrCreateUniform("ReflectFactor", osg::Uniform::Type::FLOAT)->set(0.7f);
		sphere_state_set->getOrCreateUniform("MaterialColor", osg::Uniform::Type::FLOAT_VEC4)->set(osg::Vec4f(0.5f, 0.5f, 0.5f, 1.0f));
		sphere_state_set->getOrCreateUniform("Eta", osg::Uniform::Type::FLOAT)->set(0.5f);

		sphere_state_set->setTextureAttributeAndModes(0, texture_cube_map, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);

		//-------------------------------

		root->addChild(sphere_0);

		camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

		osg::ref_ptr<osgViewer::CompositeViewer> viewer = new osgViewer::CompositeViewer();

		view->setSceneData(root.get());
		viewer->addView(view.get());

		return viewer->run();
	}
//}