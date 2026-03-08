#pragma once

#include <glad.h>

class IndexBufferObject
{
public:
	explicit IndexBufferObject(const int* indices, GLsizeiptr size);

	void Bind() const;
	void Unbind() const;
	void Delete() const;

	GLuint GetID() const { return ID; }
private:
	GLuint ID{ 0 };
};