#include "../Header/mesh.h"
#include <GL/glew.h>

Mesh::Mesh()
{
  glGenVertexArrays(1, &m_vao); // Generate a VAO
  glGenBuffers(1, &m_vbo);
  glGenBuffers(1, &m_ibo);
  m_size = 0;
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &m_vao); // Delete VAO
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ibo);
}

void Mesh::addVertices(Vertex* vertices, int vertSize, int* indices,
                       int indexSize, bool calcNormals)
{
  m_size = indexSize;

  if (calcNormals)
    this->calcNormals(vertices, vertSize, indices, indexSize);

  // Bind VAO
  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertSize * sizeof(Vertex), vertices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid*)sizeof(Vector3f));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (GLvoid*)(sizeof(Vector3f) + sizeof(Vector2f)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize * sizeof(int), indices,
               GL_STATIC_DRAW);

  glBindVertexArray(0); // Unbind VAO
}

void Mesh::draw()
{
  glBindVertexArray(m_vao);
  glDrawElements(GL_TRIANGLES, m_size, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::calcNormals(Vertex* vertices, int vertSize, int* indices,
                       int indexSize)
{
  for (int i = 0; i < indexSize; i += 3)
  {
    int i0 = indices[i];
    int i1 = indices[i + 1];
    int i2 = indices[i + 2];

    Vector3f v1 = vertices[i1].pos - vertices[i0].pos;
    Vector3f v2 = vertices[i2].pos - vertices[i0].pos;

    Vector3f normal = v1.cross(v2).normalize();

    vertices[i0].normal += normal;
    vertices[i1].normal += normal;
    vertices[i2].normal += normal;
  }

  for (int i = 0; i < vertSize; i++)
    vertices[i].normal.normalize();
}
