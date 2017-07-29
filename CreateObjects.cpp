#include "CreateObjects.h"

osg::Geode* CreateObjects::createSphere(osg::Vec3d center, double radius)
{
	// create a sphere
	osg::Geode *sphere = new osg::Geode();
	sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(center, radius)));

	return sphere;
}

osg::Node* CreateObjects::createSquare(osg::Vec3d point, double size)
{
	osg::Geometry* geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
	vertices->push_back(osg::Vec3d(point.x(), point.y(), point.z()));
	vertices->push_back(osg::Vec3d(point.x() + size, point.y(), point.z()));
	vertices->push_back(osg::Vec3d(point.x() + size, point.y() + size, point.z()));
	vertices->push_back(osg::Vec3d(point.x(), point.y() + size, point.z()));
	geom->setVertexArray(vertices.get());

	osg::Vec2Array* texcoords = new osg::Vec2Array(4);
	(*texcoords)[0].set(0.0f, 1.0f);
	(*texcoords)[1].set(0.0f, 0.0f);
	(*texcoords)[2].set(1.0f, 0.0f);
	(*texcoords)[3].set(1.0f, 1.0f);
	geom->setTexCoordArray(0, texcoords);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

	osg::Geode* geom_geode = new osg::Geode;
	geom_geode->addDrawable(geom);

	return geom_geode;

}

osg::Node* CreateObjects::createMeshSquare(osg::Vec3d point, double size, int lvl)
{

	double h = size / lvl;

	double d = h / 2.0;

	double x = point.x();
	double y = point.y();
	double z = point.z();

	osg::Geode* geom_geode = new osg::Geode;
	

	for (int j = 0; j < lvl + 1; ++j)
	{
		for (int i = 0; i < lvl + 1; ++i)
		{
			osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

			osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
			osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array();

			//vertices->push_back(osg::Vec3d(x - d, y - d, z));
			//vertices->push_back(osg::Vec3d(x - d, y + d, z));
			//vertices->push_back(osg::Vec3d(x + d, y + d, z));
			//vertices->push_back(osg::Vec3d(x + d, y - d, z));

			//texcoords->push_back(osg::Vec2d(0, 0));
			//texcoords->push_back(osg::Vec2d(0, 1));
			//texcoords->push_back(osg::Vec2d(1, 1));
			//texcoords->push_back(osg::Vec2d(1, 0));

			vertices->push_back(osg::Vec3d(x - d, y - d, z));
			vertices->push_back(osg::Vec3d(x + d, y - d, z));
			vertices->push_back(osg::Vec3d(x + d, y + d, z));
			vertices->push_back(osg::Vec3d(x - d, y + d, z));
			
			texcoords->push_back(osg::Vec2d(0, 0));
			texcoords->push_back(osg::Vec2d(1, 0));
			texcoords->push_back(osg::Vec2d(1, 1));
			texcoords->push_back(osg::Vec2d(0, 1));
			
			geom->setVertexArray(vertices.get());
			geom->setTexCoordArray(0, texcoords.get());

			osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
			(*normals)[0].set(0.0f, 0.0f, 1.0f);
			geom->setNormalArray(normals.get(), osg::Array::BIND_OVERALL);

			osg::ref_ptr<osg::Vec3Array> tangents = new osg::Vec3Array(1);
			(*tangents)[0].set(0.0f, 1.0f, 0.0f);
			geom->setVertexAttribArray(10, tangents, osg::Array::BIND_OVERALL);

			geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

			geom_geode->addDrawable(geom.get());

			x = x + h;
		}
		y = y + h;
		x = point.x();
	}
	
	//geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4 * (lvl + 1) * (lvl + 1)));

	return geom_geode;
}

osg::Node* CreateObjects::createMeshPlane(osg::Vec3d point, double size, int lvl)
{

	double h = size / lvl;

	double d = h / 2.0;

	double x = point.x();
	double y = point.y();
	double z = point.z();

	osg::Geode* geom_geode = new osg::Geode;

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array();

	for (int j = 0; j < lvl + 1; ++j)
	{
		for (int i = 0; i < lvl + 1; ++i)
		{
			vertices->push_back(osg::Vec3d(x - d, y - d, z));
			vertices->push_back(osg::Vec3d(x + d, y - d, z));
			vertices->push_back(osg::Vec3d(x + d, y + d, z));
			vertices->push_back(osg::Vec3d(x - d, y + d, z));

			double s, t, h_st;
			s = (double) i / (lvl + 1);
			t = (double) j / (lvl + 1);
			h_st = (double)1 / (lvl + 1);

			texcoords->push_back(osg::Vec2d(s, t));
			texcoords->push_back(osg::Vec2d(s + h_st, t));
			texcoords->push_back(osg::Vec2d(s + h_st, t + h_st));
			texcoords->push_back(osg::Vec2d(s, t + h_st));

			x = x + h;
		}
		y = y + h;
		x = point.x();
	}
	geom->setVertexArray(vertices.get());
	geom->setTexCoordArray(0, texcoords.get());

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
	(*normals)[0].set(0.0f, 0.0f, 1.0f);
	geom->setNormalArray(normals.get(), osg::Array::BIND_OVERALL);

	osg::ref_ptr<osg::Vec3Array> tangents = new osg::Vec3Array(1);
	(*tangents)[0].set(0.0f, 1.0f, 0.0f);
	geom->setVertexAttribArray(10, tangents, osg::Array::BIND_OVERALL);

	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4 * (lvl + 1) * (lvl + 1)));
	
	geom_geode->addDrawable(geom.get());

	return geom_geode;
}

osg::Node* CreateObjects::createBox(osg::Vec3d center, double lenX, double lenY, double lenZ)
{
	// create a box
	osg::Geode *box = new osg::Geode();
	box->addDrawable(new osg::ShapeDrawable(new osg::Box(center, lenX, lenY, lenZ)));

	return box;
}
