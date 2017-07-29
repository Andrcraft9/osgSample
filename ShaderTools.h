#ifndef SHADERSTOOLS_H
#define SHADERSTOLLS_H

#include <osg/Group>

#include <osg/StateSet>

#include <osgDB/ReadFile>

#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>

#include <osg/Program>


class ShaderTools
{
public: 
	//LOADING DIFFERENT SHADERS
	//Special
	static void attachAstronautModelShader(osg::Node* node, osg::TextureCubeMap* tex, std::string norm_tex_name,
		float reflect_factor, osg::Vec4f material_color);
	static void attachWaterShader(osg::Node* node, osg::TextureCubeMap* cubeTex, osg::Texture2D* reflectTex);
	static void attachMountainFogShader(osg::Node* node, std::string color_tex, std::string norm_tex, std::string spec_tex,
		osg::Camera* cameraToWorld, bool isHasTangent);
	// Lighting and Texture
	static void attachBlinnLighting(osg::Node* node, int lightnum);
	static void attachPhongLighting(osg::Node* node);
	static void attachRimLighting(osg::Node* node, float rimPower, float bias);
	static void attachGoochLighting(osg::Node* node);
	static void attachCookTorranceLighting(osg::Node* node);
	static void attachNormalMapShader(osg::Node* node, std::string texture, std::string normalmap, bool isHasTangent);
	static void attachParallaxMappingShader(osg::Node* node, float scale, float bias, std::string texture, 
		std::string heightmap, bool isHasTangent);
	static void attachReliefShader(osg::Node* node, float scale, float bias, std::string texture, std::string heightmap, 
		bool isHasTangent);
	static void attachSpecularNormalMapShader(osg::Node* node, std::string color_tex, std::string norm_tex, std::string spec_tex,
		bool isHasTangent);
	static void attachUltraMapShader(osg::Node* node, float scale, float bias, std::string color_tex, std::string norm_tex, 
		std::string height_tex, std::string spec_tex, bool isHasTangent);
	// Effects
	static void attachBlurReflectionShader(osg::Node* node, std::string tex_diff, osg::Texture2D* tex);
	// SkyBox
	// rf = 0.7, mc = (0.5, 0.5, 0.5, 1), e = 0.5
	static void attachSkyBoxRefletion(osg::Node* node, osg::TextureCubeMap* tex, float reflect_factor,
		osg::Vec4f material_color, float eta);


public:	
	// TOOLS
	static osg::Texture2D* createTexture(const std::string& filename);
};

#endif
