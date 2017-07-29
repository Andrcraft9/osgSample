#ifndef MATERIALTOOLS_H
#define MATERIALTOLLS_H

#include <osg/Group>

#include <osg/StateSet>
#include <osg/Material>

#include <osgDB/ReadFile>

#include <osg/Image>
#include <osg/Texture2D>
#include <osg/TextureCubeMap>

#include <osg/Program>


class MaterialTools
{
public:
	static void attachDefaultTexture(osg::Node* node, std::string texture);
	
	static void attachDefaultMaterial(osg::Node* node);
	static void attachLessAmbientMaterial(osg::Node* node);
	static void attachMyDefaultMaterial(osg::Node* node);

	static void attachStoneMaterial(osg::Node* node);
	static void attachMountainMaterial(osg::Node* node);
	static void attachMonolithMaterial(osg::Node* node);

public:
	// TOOLS
	static osg::Texture2D* createTexture(const std::string& filename);
};

#endif