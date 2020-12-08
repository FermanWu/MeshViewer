#pragma once

#include "Tri.h"
#include "Vec3f.h"
#include <vector>
#include <string>
#include "Mesh.h"

class TriMesh : public Mesh
{
public:
	TriMesh();
	~TriMesh();

	virtual void loadFile(std::string file_name) override;
	virtual void setMesh() override;
	virtual void drawMesh(QOpenGLFunctions_3_3_Core* ff, QMatrix4x4 &projection, QMatrix4x4 &model) override;

	int num_vtx_{ 0 };	// ��������
	int num_tri_{ 0 };	// ��Ƭ����

	std::vector<Vec3f> vtxs_;
	std::vector<Tri> tris_;

	QOpenGLBuffer vbo_, ebo_;
	QOpenGLVertexArrayObject vao_;

	QOpenGLShaderProgram shader_program_;
};
