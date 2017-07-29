#include <osg/Group>
#include <osg/Geometry>
#include <osg/PolygonMode>

#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

#include <osg/Program>
#include <osgGA/SphericalManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/DriveManipulator>

#include <osgUtil/TangentSpaceGenerator>

#include <osg/ClipNode>

#include "CreateObjects.h"
#include "SkyBox.h"
#include "ShaderTools.h"
#include "MaterialTools.h"
#include "RTT.h"
#include "DoomManipulator.h"

class SimpleMoveCallback : public osg::NodeCallback
{
public:
	SimpleMoveCallback(double x1, double x2) : x_min(x1), x_max(x2), x(x1), h(0.8) {}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::PositionAttitudeTransform* pat = dynamic_cast<osg::PositionAttitudeTransform*>(node);

		osg::Vec3d pos = pat->getPosition();
	
		if (x > x_max || x < x_min)
		{
			h = -h;
		}
		x = x + h;
		
		pat->setPosition(osg::Vec3d(x, pos.y(), pos.z()));
		
		// must traverse the Node's subgraph
		traverse(node, nv);
	}

private:
	double x_min;
	double x_max;

	double x;

	double h;
};

class RotateMoveCallback : public osg::NodeCallback
{
public:
	RotateMoveCallback(double x1, double x2) : x_min(x1), x_max(x2), h(0.8), phi(osg::PI/100) {}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);

		osg::Matrixd m = mt->getMatrix();
		osg::Vec3d pos = node->getBound().center();

		if (pos.x() > x_max || pos.x() < x_min)
		{
			h = -h;
		}		
		mt->setMatrix(osg::Matrixd::rotate(phi, osg::Vec3d(1, 1, 1)) * m * osg::Matrixd::translate(osg::Vec3d(h, 0, 0)));
		
		// must traverse the Node's subgraph
		traverse(node, nv);
	}

private:
	double x_min;
	double x_max;

	double h;
	double phi;
};

class LampCallback : public osg::NodeCallback
{
public:
	LampCallback(osg::Camera* camera) : _camera(camera) {}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(node);

		osg::Vec3d eye, center, up;
		_camera->getViewMatrixAsLookAt(eye, center, up);
		//std::cout << "eye" << eye.x() << " " << eye.y() << " " << eye.z() << std::endl;
		//std::cout << "center" << center.x() << " " << center.y() << " " << center.z() << std::endl;
		//std::cout << "up" << up.x() << " " << up.y() << " " << up.z() << std::endl;

		//mt->setMatrix(osg::Matrix::lookAt(eye, center, up));
		
		// must traverse the Node's subgraph
		traverse(node, nv);
	}

private:
	osg::ref_ptr<osg::Camera> _camera;
};

int main(int , char **)
{
	// Construct the viewer.
	osgViewer::Viewer viewer;

	// Create root of our scene
	osg::ref_ptr<osg::Group> root = new osg::Group();

	//////////////////////////////////// ADDING OBJECT IN OUR SCENE ////////////////////////////////////
	// SkyBox 
	//SkyBox skyBox(10000);
	SkyBox skyBox(100000);
	root->addChild(skyBox.getSkyBox());
	
	// Sphere with reflect
	//osg::ref_ptr<osg::Node> skySphere = CreateObjects::createSphere(osg::Vec3d(0, 0, -100), 40);
	//root->addChild(skySphere.get());
	//ShaderTools::attachSkyBoxRefletion(skySphere.get(), skyBox.getCubeMap(), 0.7, osg::Vec4f(0.5, 0.5, 0.5, 1), 0.5);
	
	//********************* OBJECTS *********************//
	// Astronaut
	osg::ref_ptr<osg::MatrixTransform> rootAstronaut = new osg::MatrixTransform();
	root->addChild(rootAstronaut.get());
	osg::ref_ptr<osg::Node> astronaut = osgDB::readNodeFile("models/NASA/ACES/acesjustforroomshow.obj");
	astronaut->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	rootAstronaut->addChild(astronaut.get());
	rootAstronaut->setMatrix(osg::Matrixd::scale((osg::Vec3d(50, 50, 50))) * osg::Matrixd::translate(osg::Vec3d(50, 2500, -201)));
	ShaderTools::attachAstronautModelShader(astronaut.get(), skyBox.getCubeMap(), "textures/ACES/norm.jpg", 0.7, osg::Vec4f(0.0, 0.0, 0.0, 1));
	// Astronaut2
	osg::ref_ptr<osg::MatrixTransform> rootAstronaut2 = new osg::MatrixTransform();
	root->addChild(rootAstronaut2.get());
	osg::ref_ptr<osg::Node> astronaut2 = osgDB::readNodeFile("models/Max/Max.obj");
	rootAstronaut2->addChild(astronaut2.get());
	rootAstronaut2->setMatrix(osg::Matrixd::translate(osg::Vec3d(0, -300, -201)));
	//ShaderTools::attachBlinnLighting(rootAstronaut2.get(), 0);
	rootAstronaut2->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON);
	rootAstronaut2->getOrCreateStateSet()->setMode(GL_LIGHT0, osg::StateAttribute::ON);
	rootAstronaut2->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::OFF);
	// Astronaut3
	osg::ref_ptr<osg::MatrixTransform> rootAstronaut3 = new osg::MatrixTransform();
	root->addChild(rootAstronaut3.get());
	osg::ref_ptr<osg::Node> astronaut3 = osgDB::readNodeFile("models/NASA/Mercury/mercury.obj");
	astronaut3->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	rootAstronaut3->addChild(astronaut3.get());
	rootAstronaut3->setMatrix(osg::Matrixd::scale((osg::Vec3d(50, 50, 50))) * osg::Matrixd::translate(osg::Vec3d(50, 1000, -201)));
	ShaderTools::attachRimLighting(rootAstronaut3.get(), 8.0, 0.3);
	//ShaderTools::attachAstronautModelShader(astronaut.get(), skyBox.getCubeMap(), "textures/ACES/norm.jpg", 0.7, osg::Vec4f(0.0, 0.0, 0.0, 1));
	// Space Ship
	osg::ref_ptr<osg::MatrixTransform> rootSpaceShip = new osg::MatrixTransform();
	root->addChild(rootSpaceShip.get());
	rootSpaceShip->setMatrix(osg::Matrix::rotate(-osg::PI_2, osg::Vec3(0, 0, 1)) * osg::Matrix::rotate(-osg::PI / 32.0, osg::Vec3(1, 0, 0)) * osg::Matrix::scale(35, 35, 35) *  osg::Matrixd::translate(osg::Vec3d(500, 100, -155)));
	osg::ref_ptr<osg::Node> spaceShip = osgDB::readNodeFile("models/Viper/Viper-mk-IV-fighter.obj");
	rootSpaceShip->addChild(spaceShip.get());
	ShaderTools::attachCookTorranceLighting(rootSpaceShip.get());
	// Droid
	osg::ref_ptr<osg::MatrixTransform> rootDroid = new osg::MatrixTransform();
	root->addChild(rootDroid.get());
	osg::ref_ptr<osg::Node> droid = osgDB::readNodeFile("models/ghost/Ghost_LowPoly.obj");
	rootDroid->addChild(droid.get());
	rootDroid->setMatrix(osg::Matrix::scale(osg::Vec3d(4, -4, 4)) * osg::Matrix::translate(osg::Vec3d(150, -1000, -80)));
	rootDroid->addUpdateCallback(new RotateMoveCallback(-1000, 1000));
	ShaderTools::attachBlinnLighting(rootDroid.get(), 0);
	// Reflect droid
	float z = -200.0f;
	osg::ref_ptr<osg::MatrixTransform> reverseDroid = new osg::MatrixTransform;
	reverseDroid->preMult(osg::Matrix::translate(0.0f, 0.0f, -z) *
		osg::Matrix::scale(1.0f, 1.0f, -1.0f) *
			osg::Matrix::translate(0.0f, 0.0f, z));
	reverseDroid->addChild(rootDroid.get());
	root->addChild(reverseDroid.get());
	// Create RTT camera for reflect in water
	RTT rttToDroid(osg::Camera::COLOR_BUFFER, osg::Vec4());
	osg::Camera* rttCameraToDroid = rttToDroid.getCamera();
	osg::Texture2D* rttTexDroid = rttToDroid.getTexture();
	rttCameraToDroid->addChild(reverseDroid.get());
	root->addChild(rttCameraToDroid);

	//********************* MOUNTAINS *********************//
	// Mountain
	osg::ref_ptr<osg::PositionAttitudeTransform> rootMountain = new osg::PositionAttitudeTransform();
	root->addChild(rootMountain.get());
	osg::ref_ptr<osg::Node> mountain = osgDB::readNodeFile("models/surface/MountnN14_obj.obj");
	rootMountain->addChild(mountain.get());
	rootMountain->setPosition(osg::Vec3d(6000, -1000, -200)); 
	rootMountain->setScale(osg::Vec3d(0.0001, 0.00015, 0.00015));
	MaterialTools::attachMountainMaterial(rootMountain.get());
	ShaderTools::attachSpecularNormalMapShader(rootMountain.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg", 
		"textures/mountain/Specular.jpg", false);
	// Mountain 2
	osg::ref_ptr<osg::PositionAttitudeTransform> rootMountain2 = new osg::PositionAttitudeTransform();
	root->addChild(rootMountain2.get());
	osg::ref_ptr<osg::Node> mountain2 = osgDB::readNodeFile("models/surface/MountnN14_obj.obj");
	rootMountain2->addChild(mountain2.get());
	rootMountain2->setPosition(osg::Vec3d(-6000, -1000, -200));
	rootMountain2->setScale(osg::Vec3d(0.0001, 0.00015, 0.00015));
	MaterialTools::attachMountainMaterial(rootMountain2.get());
	ShaderTools::attachSpecularNormalMapShader(rootMountain2.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg",
		"textures/mountain/Specular.jpg", false);
	// Mountain 3
	osg::ref_ptr<osg::PositionAttitudeTransform> rootMountain3 = new osg::PositionAttitudeTransform();
	root->addChild(rootMountain3.get());
	osg::ref_ptr<osg::Node> mountain3 = osgDB::readNodeFile("models/surface/MountnN14_obj.obj");
	rootMountain3->addChild(mountain3.get());
	rootMountain3->setPosition(osg::Vec3d(0, -7000, -500));
	rootMountain3->setScale(osg::Vec3d(0.0002, 0.0001, 0.0002));
	MaterialTools::attachMountainMaterial(rootMountain3.get());
	//ShaderTools::attachSpecularNormalMapShader(rootMountain3.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg",
	//	"textures/mountain/Specular.jpg", false);
	ShaderTools::attachMountainFogShader(rootMountain3.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg",
		"textures/mountain/Specular.jpg", viewer.getCamera(), false);
	// Mountain 4
	osg::ref_ptr<osg::PositionAttitudeTransform> rootMountain4 = new osg::PositionAttitudeTransform();
	root->addChild(rootMountain4.get());
	osg::ref_ptr<osg::Node> mountain4 = osgDB::readNodeFile("models/surface/MountnN14_obj.obj");
	rootMountain4->addChild(mountain4.get());
	rootMountain4->setPosition(osg::Vec3d(0, -12000, -500));
	rootMountain4->setScale(osg::Vec3d(0.0004, 0.0001, 0.0004));
	MaterialTools::attachMountainMaterial(rootMountain4.get());
	//ShaderTools::attachSpecularNormalMapShader(rootMountain4.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg",
	//	"textures/mountain/Specular.jpg", false);
	ShaderTools::attachMountainFogShader(rootMountain4.get(), "textures/mountain/ColorTex.jpg", "textures/mountain/Normal.jpg",
		"textures/mountain/Specular.jpg", viewer.getCamera(), false);
	// Reflcetions
	z = -200.0f;
	osg::ref_ptr<osg::MatrixTransform> reverseMountain = new osg::MatrixTransform;
	reverseMountain->preMult(osg::Matrix::translate(0.0f, 0.0f, -z) *
		osg::Matrix::scale(1.0f, 1.0f, -1.0f) *
			osg::Matrix::translate(0.0f, 0.0f, z));
	reverseMountain->addChild(rootMountain3.get());
	//root->addChild(reverseMountain.get());
	rttCameraToDroid->addChild(reverseMountain.get());

	//********************* STONE GROUP *********************//
	osg::ref_ptr<osg::MatrixTransform> rootStone = new osg::MatrixTransform();
	rootStone->setMatrix(osg::Matrix::scale(2, 2, 2) * osg::Matrix::translate(osg::Vec3d(-500, 0, 200)));
	root->addChild(rootStone.get());
	// Stone Cube
	osg::ref_ptr<osg::MatrixTransform> rootCube = new osg::MatrixTransform();
	rootCube->setMatrix(osg::Matrix::scale(osg::Vec3d(100, 100, 100)) * osg::Matrix::translate(osg::Vec3d(-600, 200, -100)));
	osg::ref_ptr<osg::Node> cube = osgDB::readNodeFile("models/stone/cube.obj");
	rootCube->addChild(cube.get());
	rootStone->addChild(rootCube.get());
	ShaderTools::attachUltraMapShader(rootCube.get(), 0.05, 0, "textures/ultra/stone/s1_diffuse.png",
		"textures/ultra/stone/s1_normal.png", "textures/ultra/stone/s1_heights.png", "textures/ultra/stone/s1_specular.png", false);
	// Stone Sphere
	osg::ref_ptr<osg::MatrixTransform> rootSphere = new osg::MatrixTransform();
	rootSphere->setMatrix(osg::Matrix::scale(osg::Vec3d(20, 20, 20)) * osg::Matrix::translate(osg::Vec3d(-450, 150, -180)));
	osg::ref_ptr<osg::Node> sphere = osgDB::readNodeFile("models/stone/blue_sphere.obj");
	rootSphere->addChild(sphere.get());
	rootStone->addChild(rootSphere.get());
	ShaderTools::attachNormalMapShader(rootSphere.get(), "textures/bumpmap/Rock01.tga", "textures/bumpmap/Rock01_n.tga", false);
	//ShaderTools::attachNormalMapShader(rootSphere.get(), "textures/bumpmap/sand.jpg", "textures/bumpmap/sand_norm.jpg");
	
	//********************* RELIEF *********************//
	// Relief stone
	osg::ref_ptr<osg::MatrixTransform> rootRelief = new osg::MatrixTransform();
	root->addChild(rootRelief.get());
	osg::ref_ptr<osg::Node> relief = osgDB::readNodeFile("models/rocks/rocks_01_model.obj");
	rootRelief->addChild(relief.get());
	rootRelief->setMatrix(osg::Matrix::scale(2, 2, 2) * osg::Matrix::translate(osg::Vec3d(-1200, -1500, -200)));
	ShaderTools::attachSpecularNormalMapShader(rootRelief.get(), "models/rocks/rocks_01_dif.tga",
		"models/rocks/rocks_01_nm.tga", "models/rocks/rocks_01_spec.tga", false);
	//rootRelief->setMatrix(osg::Matrix::scale(3, 3, 3) * osg::Matrix::translate(osg::Vec3d(-600, 400, -180)));
	z = -200.0f;
	osg::ref_ptr<osg::MatrixTransform> reverseRelief = new osg::MatrixTransform;
	reverseRelief->preMult(osg::Matrix::translate(0.0f, 0.0f, -z) *
		osg::Matrix::scale(1.0f, 1.0f, -1.0f) *
			osg::Matrix::translate(0.0f, 0.0f, z));
	reverseRelief->addChild(rootRelief.get());
	//root->addChild(reverseRelief.get());
	rttCameraToDroid->addChild(reverseRelief.get());

	//********************* OTHER *********************//
	// Metal Cube
	osg::ref_ptr<osg::MatrixTransform> rootMetalCube = new osg::MatrixTransform();
	rootMetalCube->setMatrix(osg::Matrix::scale(osg::Vec3d(20, 20, 20)) * osg::Matrix::translate(osg::Vec3d(100, -300, -180)));
	osg::ref_ptr<osg::Node> metalCube = osgDB::readNodeFile("models/stone/cube.obj");
	rootMetalCube->addChild(metalCube.get());
	root->addChild(rootMetalCube.get());
	ShaderTools::attachUltraMapShader(rootMetalCube.get(), 0.05, 0, "textures/ultra/Metal_Cover/Metal_Cover_1k_d.tga",
		"textures/ultra/Metal_Cover/Metal_Cover_1k_n.tga", "textures/ultra/Metal_Cover/Metal_Cover_1k_h.tga", "textures/ultra/Metal_Cover/Metal_Cover_1k_s.tga", false);
	// Monolith 
	osg::ref_ptr<osg::MatrixTransform> rootMonolith = new osg::MatrixTransform();
	root->addChild(rootMonolith.get());
	osg::ref_ptr<osg::Node> monolith = CreateObjects::createBox(osg::Vec3d(0, 0, 0), 8, 1.5, 20);
	rootMonolith->addChild(monolith.get());
	rootMonolith->setMatrix(osg::Matrix::scale(15, 15, 15) * osg::Matrix::translate(osg::Vec3d(0, 1500, -30)));
	MaterialTools::attachMonolithMaterial(rootMonolith.get());
	//ShaderTools::attachRimLighting(rootMonolith.get(), 5.0, 1.0);
	
	// Reflcet Monolith
	z = -200.0f;
	osg::ref_ptr<osg::MatrixTransform> reverseMonolith = new osg::MatrixTransform;
	reverseMonolith->preMult(osg::Matrix::translate(0.0f, 0.0f, -z) *
		osg::Matrix::scale(1.0f, 1.0f, -1.0f) *
		osg::Matrix::translate(0.0f, 0.0f, z));
	reverseMonolith->addChild(rootMonolith.get());
	root->addChild(reverseMonolith.get());
	// Create RTT camera for reflect in stand plane
	RTT rttToMonolith(osg::Camera::COLOR_BUFFER, osg::Vec4(0.2, 0.2, 0.2, 1));
	osg::Camera* rttCameraToMonolith = rttToMonolith.getCamera();
	osg::Texture2D* rttTexMonolith = rttToMonolith.getTexture();
	rttCameraToMonolith->addChild(reverseMonolith.get());
	root->addChild(rttCameraToMonolith);
	// Stand Monolith Plane
	osg::ref_ptr<osg::Node> standMonolithPlane = CreateObjects::createMeshPlane(osg::Vec3d(-400, 1100, -198), 800, 20);
	root->addChild(standMonolithPlane.get());
	ShaderTools::attachBlurReflectionShader(standMonolithPlane.get(), "textures/MetalBasic01.tga", rttTexMonolith);

	//********************* SURFACE *********************//
	// Water plane
	osg::ref_ptr<osg::Node> waterPlane = CreateObjects::createMeshPlane(osg::Vec3d(-2000, -4600, -201), 4000, 20);
	root->addChild(waterPlane.get());
	ShaderTools::attachWaterShader(waterPlane.get(), skyBox.getCubeMap(), rttTexDroid);
	// Stand Plane
	//osg::ref_ptr<osg::Node> standPlane = CreateObjects::createSquare(osg::Vec3d(-10000, -15000, -202), 20000);
	osg::ref_ptr<osg::Node> standPlane = CreateObjects::createMeshSquare(osg::Vec3d(-10000, -15000, -202), 20000, 50);
	root->addChild(standPlane.get());
	MaterialTools::attachLessAmbientMaterial(standPlane.get());
	MaterialTools::attachDefaultTexture(standPlane.get(), "textures/ground002.jpg");
	standMonolithPlane->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
	//ShaderTools::attachPhongLighting(standPlane.get());
	//MaterialTools::attachDefaultTexture(standPlane.get(), "textures/ground002.jpg");

	////////////////////////////////////////// LET BE THE LIGHT //////////////////////////////////////////
	// Main light source
	osg::ref_ptr<osg::PositionAttitudeTransform> rootLight = new osg::PositionAttitudeTransform();
	root->addChild(rootLight.get());
	osg::ref_ptr<osg::LightSource> ls = new osg::LightSource();
	rootLight->addChild(ls.get());
	{
		// set Light
		osg::ref_ptr<osg::Light> light = new osg::Light();
		light->setAmbient(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));
		light->setDiffuse(osg::Vec4(0.7f, 0.7f, 0.7f, 1.f));
		light->setSpecular(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));
		light->setPosition(osg::Vec4(0, 0, 0, 1));
		light->setLightNum(0);
		ls->setLight(light.get());
		// create a sphere
		osg::Geode *ls_shape = new osg::Geode();
		ls_shape->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0, 0, 0), 5)));
		rootLight->addChild(ls_shape);
		osg::StateSet* stateset = ls_shape->getOrCreateStateSet();
		osg::ref_ptr<osg::Material> mat = new osg::Material();
		mat->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 1, 1));
		stateset->setAttribute(mat.get());
	}
	rootLight->setPosition(osg::Vec3d(0, 0, 300));
	//rootLight->setPosition(osg::Vec3d(150, -2000, -50));
	//rootLight->addUpdateCallback(new SimpleMoveCallback(-300, 300));
	// Lamp
	osg::ref_ptr<osg::PositionAttitudeTransform> rootLamp = new osg::PositionAttitudeTransform();;
	root->addChild(rootLamp.get());
	osg::ref_ptr<osg::LightSource> lampSource = new osg::LightSource;
	rootLamp->addChild(lampSource.get());
	{
		// set Light
		osg::ref_ptr<osg::Light> light = new osg::Light();
		light->setAmbient(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));
		light->setDiffuse(osg::Vec4(0.4f, 0.4f, 0.4f, 1.f));
		light->setSpecular(osg::Vec4(0.8f, 0.8f, 0.8f, 1.f));
		light->setPosition(osg::Vec4(0, 0, 0, 1));
		light->setLightNum(1);
		// create a cube
		lampSource->setLight(light.get());
		osg::MatrixTransform* mt = new osg::MatrixTransform();
		osg::ref_ptr<osg::Node> box = osgDB::readNodeFile("models/stone/cube.obj");
		mt->addChild(box.get());
		mt->setMatrix(osg::Matrix::scale(2, 2, 2));
		rootLamp->addChild(mt);
	}
	rootLamp->setPosition(osg::Vec3d(50, 1600, 500));
	// Sun
	osg::ref_ptr<osg::PositionAttitudeTransform> rootSun = new osg::PositionAttitudeTransform();
	root->addChild(rootSun.get());
	osg::ref_ptr<osg::LightSource> lsSun = new osg::LightSource();
	rootSun->addChild(lsSun.get());
	{
		// set Light
		osg::ref_ptr<osg::Light> light = new osg::Light();
		light->setAmbient(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));
		light->setDiffuse(osg::Vec4(0.7f, 0.7f, 0.7f, 1.f));
		light->setSpecular(osg::Vec4(0.2f, 0.2f, 0.2f, 1.f));
		light->setPosition(osg::Vec4(0, 0, 0, 1));
		light->setLightNum(2);
		lsSun->setLight(light.get());
	}
	rootSun->setPosition(osg::Vec3d(0, 3000, 1000));
	
	///////////////////////////////////////// SHOW SCENE ///////////////////////////////////////// 

	// Configure root
	root->getOrCreateStateSet()->setMode(GL_LIGHT2, osg::StateAttribute::ON);
	//root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	// set the scene to render
	viewer.setSceneData(root.get());
	//viewer.setLightingMode(osg::View::LightingMode::NO_LIGHT);

	/*
	osgGA::SphericalManipulator * s_m = new osgGA::SphericalManipulator();
	s_m->setMinimumZoomScale(0);
	s_m->setCenter(osg::Vec3d(0, 1500, -150));
	viewer.setCameraManipulator(s_m);
	*/

	DoomManipulator* doom = new DoomManipulator();
	doom->setHomePosition(osg::Vec3d(0, 1, 0), osg::Vec3d(0, 0, -100), osg::Vec3d(0, 0, 1));
	viewer.setCameraManipulator(doom);

	viewer.getCamera()->setClearColor(osg::Vec4d());

	
	return viewer.run();
}

