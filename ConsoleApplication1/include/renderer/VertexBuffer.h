#pragma once

#include <glad.h>

class VertexBufferObject
{
public:
	explicit VertexBufferObject(const GLfloat* vertices, GLsizeiptr size);

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	GLuint GetID() const { return ID; }
private:
	GLuint ID{0};
};