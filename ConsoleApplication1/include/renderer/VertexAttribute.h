#pragma once

#include <glad.h>
#include <renderer/VertexBuffer.h>

class VertexAttributeObject
{
public:
	explicit VertexAttributeObject();

	void LinkVBO( const VertexBufferObject& vbo, 
				  GLsizei size,
				  GLsizei stride,
				  GLuint layout ) const;
	void Bind() const;
	void Unbind() const;
	void Delete() const;

	GLuint GetID() const { return ID; }
private:
	GLuint ID{ 0 };
};