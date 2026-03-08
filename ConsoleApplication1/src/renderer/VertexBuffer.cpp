#include <renderer/VertexBuffer.h>

VertexBufferObject::VertexBufferObject(const GLfloat* vertices, GLsizeiptr size) 
{
	glGenBuffers(1, &ID) ;
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBufferObject::Bind() const 
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBufferObject::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBufferObject::Delete() const
{
	glDeleteBuffers(1, &ID);
}
