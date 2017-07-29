#include "MaterialTools.h"

osg::Texture2D* MaterialTools::createTexture(const std::string& filename)
{
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(osgDB::readImageFile(filename));
	texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
	texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
	texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	return texture.release();
}

void MaterialTools::attachDefaultTexture(osg::Node* node, std::string texture)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();
	stateset->setTextureAttributeAndModes(0, createTexture(texture), osg::StateAttribute::ON);

	return;
}

void MaterialTools::attachDefaultMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	stateset->setAttribute(material.get());
}

void MaterialTools::attachLessAmbientMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4d(0.1, 0.1, 0.1, 1)); // Рассеянный цвет материала (цвет в тени)
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.8f, 0.8f, 1.0f, 1.0f)); // Диффузный цвет материала
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.1f, 0.1f, 0.1f, 0.1f)); // Зеркальный цвет материала
	material->setShininess(osg::Material::FRONT_AND_BACK, 200.0f); // Степень зеркально отражения материала (0-128)
	material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1)); // Интенсивность излучаемого цвета материала
	
	stateset->setAttribute(material.get());
}

void MaterialTools::attachMyDefaultMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4d(0.2f, 0.2f, 0.2f, 1)); // Рассеянный цвет материала (цвет в тени)
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.5f, 0.5f, 0.5f, 1.f)); // Диффузный цвет материала
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.7f, 0.7f, 0.7f, 1.0f)); // Зеркальный цвет материала
	material->setShininess(osg::Material::FRONT_AND_BACK, 30.0f); // Степень зеркально отражения материала (0-128)
	material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1)); // Интенсивность излучаемого цвета материала

	stateset->setAttribute(material.get());
}

void MaterialTools::attachStoneMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4d(0.2f, 0.2f, 0.2f, 1)); // Рассеянный цвет материала (цвет в тени)
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.6f, 0.7f, 0.8f, 1.f)); // Диффузный цвет материала
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3f, 0.3f, 0.32f, 1.0f)); // Зеркальный цвет материала
	material->setShininess(osg::Material::FRONT_AND_BACK, 20.0f); // Степень зеркально отражения материала (0-128)
	material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1)); // Интенсивность излучаемого цвета материала

	stateset->setAttribute(material.get());

}

void MaterialTools::attachMountainMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4d(0.2f, 0.2f, 0.2f, 1)); // Рассеянный цвет материала (цвет в тени)
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.5f, 0.5f, 0.5f, 1.f)); // Диффузный цвет материала
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.3f, 0.3f, 0.3f, 1.0f)); // Зеркальный цвет материала
	material->setShininess(osg::Material::FRONT_AND_BACK, 30.0f); // Степень зеркально отражения материала (0-128)
	material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1)); // Интенсивность излучаемого цвета материала

	stateset->setAttribute(material.get());
}

void MaterialTools::attachMonolithMaterial(osg::Node* node)
{
	osg::StateSet* stateset = node->getOrCreateStateSet();

	osg::ref_ptr<osg::Material> material = new osg::Material();
	material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4d(0.0f, 0.0f, 0.0f, 1)); // Рассеянный цвет материала (цвет в тени)
	material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f)); // Диффузный цвет материала
	material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f)); // Зеркальный цвет материала
	material->setShininess(osg::Material::FRONT_AND_BACK, 100.0f); // Степень зеркально отражения материала (0-128)
	material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 0, 1)); // Интенсивность излучаемого цвета материала

	stateset->setAttribute(material.get());

}