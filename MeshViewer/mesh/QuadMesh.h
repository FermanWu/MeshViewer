#pragma once
#include "Mesh.h"
#include "Vec3f.h"
#include "Quad.h"

class QuadMesh : public Mesh
{
public:
	QuadMesh();
	~QuadMesh();

	virtual void loadFile(std::string file_name) override;
	virtual void setMesh() override;
	virtual void drawMesh(QOpenGLFunctions_3_3_Core* ff, QMatrix4x4& projection, QMatrix4x4& model) override;
	virtual bool isEmpty() override;

	int num_vtx_{ 0 };	// 顶点数量
	int num_quad_{ 0 };	// 面片数量

	std::vector<Vec3f> vtxs_;
	std::vector<Quad> quads_;

	QOpenGLBuffer vbo_, ebo_;
	QOpenGLVertexArrayObject vao_;

	QOpenGLShaderProgram shader_program_;

	REAL scale_factor_{ 1 };
	QVector3D translate_;
};

