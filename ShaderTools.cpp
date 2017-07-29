#include "ShaderTools.h"
#include <osg/TexGen>

#include <osgUtil/TangentSpaceGenerator>
#include <osg/MatrixTransform>
#include <osg/Camera>

class UpdateMatrixTransformUniform : public osg::Uniform::Callback
{
public:
	UpdateMatrixTransformUniform(osg::MatrixTransform* mt) : _mt(mt) { }

	virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* nv)
	{
		uniform->set(_mt->getMatrix());	
	}

private:
	osg::ref_ptr<osg::MatrixTransform> _mt;
};

class UpdateCameraUniform : public osg::Uniform::Callback
{
public:
	UpdateCameraUniform(osg::Camera* camera) : _camera(camera) { }

	virtual void operator()(osg::Uniform* uniform, osg::NodeVisitor* nv)
	{
		uniform->set(_camera->getInverseViewMatrix());
	}

private:
	osg::ref_ptr<osg::Camera> _camera;
};


class ComputeTangentVisitor : public osg::NodeVisitor
{
public:
	void apply(osg::Node& node) { traverse(node); }

	void apply(osg::Geode& node)
	{
		for (unsigned int i = 0; i<node.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(node.getDrawable(i));
			if (geom) generateTangentArray(geom);
		}
		traverse(node);
	}

	void generateTangentArray(osg::Geometry* geom)
	{
		osg::ref_ptr<osgUtil::TangentSpaceGenerator> tsg = new osgUtil::TangentSpaceGenerator;
		tsg->generate(geom, 0);
		geom->setVertexAttribArray(10, tsg->getTangentArray());
		geom->setVertexAttribBinding(10, osg::Geometry::BIND_PER_VERTEX);
		//std::cout << "WORK!\n";
	}
};

class NodeShaderCallback : public osg::NodeCallback
{
public:
	NodeShaderCallback(osg::Camera *camera) : _camera(camera) {}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		node->getOrCreateStateSet()->getOrCreateUniform("CameraToWorld", osg::Uniform::FLOAT_MAT4)->set(_camera->getInverseViewMatrix());

		// must traverse the Node's subgraph
		traverse(node, nv);
	}

private:
	osg::Camera* _camera;
};

osg::Texture2D* ShaderTools::createTexture(const std::string& filename)
{

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(filename));
	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	return texture.release();
}

void ShaderTools::attachAstronautModelShader(osg::Node* node, osg::TextureCubeMap* cub_tex, std::string norm_tex_name, 
	float reflect_factor, osg::Vec4f material_color)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	stateset->setTextureAttributeAndModes(0, createTexture(norm_tex_name), value);
	stateset->setTextureAttributeAndModes(1, cub_tex, value);

	// Generate tangents
	ComputeTangentVisitor ctv;
	ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	node->accept(ctv);

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/special/aces.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/special/aces.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	program->addBindAttribLocation("Tangent", 10);

	stateset->setAttributeAndModes(program.get());
	
	stateset->getOrCreateUniform("ReflectFactor", osg::Uniform::Type::FLOAT)->set(reflect_factor);
	stateset->getOrCreateUniform("MaterialColor", osg::Uniform::Type::FLOAT_VEC4)->set(material_color);

	node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalMapTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("CubeMapTex", 1));

	return;
}

void ShaderTools::attachWaterShader(osg::Node* node, osg::TextureCubeMap* cubeTex, osg::Texture2D* reflectTex)
{
	osg::StateSet* ss = node->getOrCreateStateSet();
	
	ss->setTextureAttributeAndModes(0, createTexture("textures/water/skymap.bmp"));
	ss->setTextureAttributeAndModes(1, createTexture("textures/water/water_DUDV.jpg"));
	ss->setTextureAttributeAndModes(2, createTexture("textures/water/water_NM.jpg"));
	ss->setTextureAttributeAndModes(3, reflectTex);
	ss->setTextureAttributeAndModes(4, cubeTex);
	//ss->setTextureAttributeAndModes(1, createTexture("textures/water/Water/water_DUDV.tga"));
	//ss->setTextureAttributeAndModes(2, createTexture("textures/water/Water/water_normal.jpg"));

	osg::ref_ptr<osg::Program> program = new osg::Program;
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/water/water.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/water/water.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	node->getOrCreateStateSet()->setAttributeAndModes(program.get());
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("DefaultMapTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("Refraction", 1));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalTex", 2));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("Reflection", 3));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("CubeMapTexture", 4));

	return;
}

void ShaderTools::attachMountainFogShader(osg::Node* node, std::string color_tex, std::string norm_tex, std::string spec_tex,
	osg::Camera* camera, bool isHasTangent)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	stateset->setTextureAttributeAndModes(0, createTexture(color_tex), value);
	stateset->setTextureAttributeAndModes(1, createTexture(norm_tex), value);
	stateset->setTextureAttributeAndModes(2, createTexture(spec_tex), value);

	if (!isHasTangent)
	{
		// Generate tangents
		ComputeTangentVisitor ctv;
		ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		node->accept(ctv);
	}

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/special/mountain_fog.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/special/mountain_fog.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	program->addBindAttribLocation("Tangent", 10);

	stateset->setAttributeAndModes(program.get());

	node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalMapTex", 1));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("SpecularMapTex", 2));

	osg::Matrixd mat;
	mat.set(1, 1, 1, 1, 
			1, 0, 0, 1, 
			0, 1, 0, 1, 
			0, 0, 1, 1);
	
	//osg::Uniform *uniMatrix = new osg::Uniform("CameraToWorld", mat);
	//osg::Uniform *uniMatrix = new osg::Uniform("CameraToWorld", osg::Uniform::FLOAT_MAT4);
	//uniMatrix->set(mat);
	//node->getOrCreateStateSet()->addUniform(uniMatrix);

	node->getOrCreateStateSet()->getOrCreateUniform("CameraToWorld", osg::Uniform::FLOAT_MAT4)->set(camera->getInverseViewMatrix());
	node->addUpdateCallback(new NodeShaderCallback(camera));

	//node->getOrCreateStateSet()->getOrCreateUniform("CameraToWorld", osg::Uniform::FLOAT_MAT4)->set(mat);

	//uniMatrix->setUpdateCallback(new UpdateCameraUniform(camera));

	return;
}

void ShaderTools::attachBlinnLighting(osg::Node* node, int lightnum)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/lighting/blinn.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/lighting/blinn.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	stateset->getOrCreateUniform("LightNum", osg::Uniform::Type::INT)->set(lightnum);

	return;
}

void ShaderTools::attachPhongLighting(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/lighting/phong.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/lighting/phong.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	return;
}

void ShaderTools::attachRimLighting(osg::Node* node, float rimPower, float bias)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/lighting/rim.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/lighting/rim.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	stateset->getOrCreateUniform("RimPower", osg::Uniform::Type::FLOAT)->set(rimPower);
	stateset->getOrCreateUniform("Bias", osg::Uniform::Type::FLOAT)->set(bias);

	return;
}

void ShaderTools::attachGoochLighting(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/lighting/gooch.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/lighting/gooch.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	return;
}

void ShaderTools::attachCookTorranceLighting(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/lighting/cook-torrance2.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/lighting/cook-torrance2.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());
	stateset->setAttributeAndModes(program.get());

	return;
}

void ShaderTools::attachNormalMapShader(osg::Node* node, std::string texture_name, std::string normalmap_name, bool isHasTangent)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;
	
	stateset->setTextureAttributeAndModes(0, createTexture(texture_name), value);
	stateset->setTextureAttributeAndModes(1, createTexture(normalmap_name), value);
	
	if (!isHasTangent)
	{
		// Generate tangents
		ComputeTangentVisitor ctv;
		ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		node->accept(ctv);
	}

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/texture/normal_map.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/texture/normal_map.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	program->addBindAttribLocation("Tangent", 10);

	stateset->setAttributeAndModes(program.get());
	
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalMapTex", 1));

	return;
}

void ShaderTools::attachParallaxMappingShader(osg::Node* node, float scale, float bias, std::string texture, 
	std::string heightmap, bool isHasTangent)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	osg::ref_ptr<osg::Texture2D> htex = new osg::Texture2D;
	osg::Image* himg = osgDB::readImageFile(heightmap);

	if (himg == NULL)
		std::cout << "NULL\n";
	
	htex->setImage(himg);

	stateset->setTextureAttributeAndModes(0, createTexture(texture), value);
	stateset->setTextureAttributeAndModes(1, htex, value);

	if (!isHasTangent)
	{
		// Generate tangents
		ComputeTangentVisitor ctv;
		ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		node->accept(ctv);
	}

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/texture/parallax_map.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/texture/parallax_map.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	program->addBindAttribLocation("Tangent", 10);

	stateset->setAttributeAndModes(program.get());

	node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("HeightMapTex", 1));
	stateset->getOrCreateUniform("Scale", osg::Uniform::Type::FLOAT)->set(scale);
	stateset->getOrCreateUniform("Bias", osg::Uniform::Type::FLOAT)->set(bias);

	return;
}

 void ShaderTools::attachReliefShader(osg::Node* node, float scale, float bias, std::string texture, std::string heightmap,
	 bool isHasTangent)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	/*
	osg::ref_ptr<osg::Texture2D> htex = new osg::Texture2D;
	osg::Image* himg = osgDB::readImageFile(heightmap);
	if (himg == NULL)
		std::cout << "NULL\n";
	htex->setImage(himg);
	*/

	stateset->setTextureAttributeAndModes(0, createTexture(texture), value);
	stateset->setTextureAttributeAndModes(1, createTexture(heightmap), value);

	if (!isHasTangent)
	{
		// Generate tangents
		ComputeTangentVisitor ctv;
		ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		node->accept(ctv);
	}

	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/texture/relief.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/texture/relief.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	program->addBindAttribLocation("Tangent", 10);

	stateset->setAttributeAndModes(program.get());

	node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	node->getOrCreateStateSet()->addUniform(new osg::Uniform("HeightMapTex", 1));
	stateset->getOrCreateUniform("Scale", osg::Uniform::Type::FLOAT)->set(scale);
	stateset->getOrCreateUniform("Bias", osg::Uniform::Type::FLOAT)->set(bias);

	return;

}

 void ShaderTools::attachSpecularNormalMapShader(osg::Node* node, std::string color_tex, std::string norm_tex, 
	 std::string spec_tex, bool isHasTangent)
 {
	 osg::StateSet* stateset = node->getOrCreateStateSet();

	 osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	 stateset->setTextureAttributeAndModes(0, createTexture(color_tex), value);
	 stateset->setTextureAttributeAndModes(1, createTexture(norm_tex), value);
	 stateset->setTextureAttributeAndModes(2, createTexture(spec_tex), value);

	 if (!isHasTangent)
	 {
		 // Generate tangents
		 ComputeTangentVisitor ctv;
		 ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		 node->accept(ctv);
	 }

	 osg::ref_ptr<osg::Program> program = new osg::Program();
	 osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/texture/specular_map.vert");
	 osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/texture/specular_map.frag");
	 program->addShader(shader_vertex.get());
	 program->addShader(shader_fragment.get());

	 program->addBindAttribLocation("Tangent", 10);

	 stateset->setAttributeAndModes(program.get());

	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalMapTex", 1));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("SpecularMapTex", 2));

	 return;
 }

 void ShaderTools::attachUltraMapShader(osg::Node* node, float scale, float bias, std::string color_tex, std::string norm_tex,
	 std::string height_tex, std::string spec_tex, bool isHasTangent)
 {
	 osg::StateSet* stateset = node->getOrCreateStateSet();

	 osg::StateAttribute::GLModeValue value = osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE;

	 stateset->setTextureAttributeAndModes(0, createTexture(color_tex), value);
	 stateset->setTextureAttributeAndModes(1, createTexture(norm_tex), value);
	 stateset->setTextureAttributeAndModes(2, createTexture(height_tex), value);
	 stateset->setTextureAttributeAndModes(3, createTexture(spec_tex), value);

	 if (!isHasTangent)
	 {
		 // Generate tangents
		 ComputeTangentVisitor ctv;
		 ctv.setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
		 node->accept(ctv);
	 }

	 osg::ref_ptr<osg::Program> program = new osg::Program();
	 osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/texture/ultra_map.vert");
	 osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/texture/ultra_map.frag");
	 program->addShader(shader_vertex.get());
	 program->addShader(shader_fragment.get());

	 program->addBindAttribLocation("Tangent", 10);

	 stateset->setAttributeAndModes(program.get());

	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("ColorTex", 0));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("NormalMapTex", 1));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("HeightMapTex", 2));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("SpecularMapTex", 3));
	 stateset->getOrCreateUniform("Scale", osg::Uniform::Type::FLOAT)->set(scale);
	 stateset->getOrCreateUniform("Bias", osg::Uniform::Type::FLOAT)->set(bias);

	 return;
 }

 void ShaderTools::attachBlurReflectionShader(osg::Node* node, std::string tex_diff, osg::Texture2D* tex)
 {
	 osg::StateSet* stateset = node->getOrCreateStateSet();

	 stateset->setTextureAttributeAndModes(0, createTexture(tex_diff), osg::StateAttribute::ON);
	 stateset->setTextureAttributeAndModes(1, tex, osg::StateAttribute::ON);

	 osg::ref_ptr<osg::Program> program = new osg::Program();
	 osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/effect/blur_reflect.vert");
	 osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/effect/blur_reflect.frag");
	 program->addShader(shader_vertex.get());
	 program->addShader(shader_fragment.get());

	 stateset->setAttributeAndModes(program.get());

	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("DiffTexture", 0));
	 node->getOrCreateStateSet()->addUniform(new osg::Uniform("Reflection", 1));
 }


void ShaderTools::attachSkyBoxRefletion(osg::Node* node, osg::TextureCubeMap* tex, float reflect_factor,
	osg::Vec4f material_color, float eta)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	stateset->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
	osg::ref_ptr<osg::Program> program = new osg::Program();
	osg::ref_ptr<osg::Shader> shader_vertex = osgDB::readShaderFile("shaders/cube_map.vert");
	osg::ref_ptr<osg::Shader> shader_fragment = osgDB::readShaderFile("shaders/cube_map.frag");
	program->addShader(shader_vertex.get());
	program->addShader(shader_fragment.get());

	stateset->setAttributeAndModes(program.get());

	stateset->getOrCreateUniform("ReflectFactor", osg::Uniform::Type::FLOAT)->set(reflect_factor);
	stateset->getOrCreateUniform("MaterialColor", osg::Uniform::Type::FLOAT_VEC4)->set(material_color);
	stateset->getOrCreateUniform("Eta", osg::Uniform::Type::FLOAT)->set(eta);

	return;
}