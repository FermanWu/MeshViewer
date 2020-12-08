#include "TriMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <QOpenGLWidget>

TriMesh::TriMesh(): vbo_(QOpenGLBuffer::VertexBuffer),
ebo_(QOpenGLBuffer::IndexBuffer)
{

}

TriMesh::~TriMesh()
{
	vao_.destroy();
	vbo_.destroy();
	ebo_.destroy();

}

void TriMesh::loadFile(std::string file_name)
{
	std::ifstream in(file_name);

	if (!in)
	{
		std::cout << "The file can not be opened!" << std::endl;
		return;
	}
	
	std::string line;
	while (std::getline(in, line))
	{
		std::string tag;
		std::istringstream is(line);
		
		is >> tag;
		if (tag == "v")
		{
			Vec3f vec3f;
			is >> vec3f.x >> vec3f.y >> vec3f.z;
			vtxs_.push_back(vec3f);
		}
		else if (tag == "f")
		{
			Tri tri;
			is >> tri.v[0] >> tri.v[1] >> tri.v[2];
			tri.v[0] --;
			tri.v[1] --;
			tri.v[2] --;
			tris_.push_back(tri);
		}
	}
	in.close();

	num_vtx_ = vtxs_.size();
	num_tri_ = tris_.size();
}

void TriMesh::setMesh()
{
	//  ������ɫ��
	bool success = shader_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "GLSL/triangle.vert");
	if (!success)
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader_program_.log();
		return;
	}

	// Ƭ����ɫ��
	success = shader_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "GLSL/triangle.frag");
	if (!success)
	{
		qDebug() << "shaderProgram addShaderFromSourceFile failed!" << shader_program_.log();
		return;
	}

	// ������ɫ��
	success = shader_program_.link();
	if (!success)
	{
		qDebug() << "shaderProgram link failed!" << shader_program_.log();
		return;
	}

	vao_.create();
	vao_.bind();

	// ����������vbo����
	vbo_.create();
	vbo_.bind();
	vbo_.allocate(&vtxs_[0], vtxs_.size() * sizeof(Vec3f));

	// ����������ebo����
	ebo_.create();
	ebo_.bind();
	ebo_.allocate(&tris_[0], tris_.size() * sizeof(Tri));

	int attr = -1;
	attr = shader_program_.attributeLocation("aPos");
	shader_program_.setAttributeBuffer(attr, GL_FLOAT, 0, 3, sizeof(GLfloat) * 3);	// ���ý������ݵķ�ʽ
	shader_program_.enableAttributeArray(attr);

	vbo_.release();

	shader_program_.bind();
	// ��ͼview�任����
	QMatrix4x4 view;
	view.lookAt(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
	shader_program_.setUniformValue("view", view);

}

void TriMesh::drawMesh(QOpenGLFunctions_3_3_Core* ff, QMatrix4x4 &projection, QMatrix4x4 &model)
{
	shader_program_.bind();
	shader_program_.setUniformValue("projection", projection);
	shader_program_.setUniformValue("model", model);

	vao_.bind();

	ff->glDrawElements(GL_TRIANGLES, tris_.size() * 3, GL_UNSIGNED_INT, 0);		// ����

	shader_program_.release();
}