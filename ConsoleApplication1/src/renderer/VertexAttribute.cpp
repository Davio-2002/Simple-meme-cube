#include <renderer/VertexAttribute.h>

VertexAttributeObject::VertexAttributeObject()
{
	glGenVertexArrays(1, &ID);
}

void VertexAttributeObject::LinkVBO(const VertexBufferObject& vbo, GLsizei size, GLsizei stride, GLuint layout) const
{
	vbo.Bind();
	glVertexAttribPointer(layout, size, GL_FLOAT, GL_FALSE, stride, nullptr);
	glEnableVertexAttribArray(layout);
	vbo.Unbind();
}

void VertexAttributeObject::Bind() const
{
	glBindVertexArray(ID);
}

void VertexAttributeObject::Unbind() const
{
	glBindVertexArray(0);
}

void VertexAttributeObject::Delete() const
{
	glDeleteVertexArrays(1, &ID);
}
