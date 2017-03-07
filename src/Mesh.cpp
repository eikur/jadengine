#include "Mesh.h"
#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>
#include <vector>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Material.h"

Mesh::Mesh()
{}

Mesh::Mesh(aiMesh* mesh, Material* material)
	: m_material(material)
{
	m_num_elements = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		std::vector<float3> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			vertices.push_back(float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		}

		glGenBuffers(1, &m_vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, size(vertices) * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
		
		vertices.clear();
	}

	if (mesh->HasTextureCoords(0)) {
		std::vector<float2> texture_coords;
		texture_coords.reserve(mesh->mNumVertices);
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			texture_coords.push_back(float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}

		glGenBuffers(1, &m_vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, size(texture_coords) * sizeof(texture_coords[0]), &texture_coords[0], GL_STATIC_DRAW);

		texture_coords.clear();
	}

	if (mesh->HasNormals()) {
		std::vector<float3> normals;
		normals.reserve(mesh->mNumVertices);
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			normals.push_back(float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
		}

		glGenBuffers(1, &m_vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, size(normals) * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);

		normals.clear();
	}

	if (mesh->HasFaces()) {
		std::vector<GLuint> indices;
		indices.reserve(mesh->mNumFaces * mesh->mFaces[0].mNumIndices);
		for (size_t i = 0; i < mesh->mNumFaces; ++i) {
			for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		glGenBuffers(1, &m_vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size(indices) * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		indices.clear();
	}

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Mesh::~Mesh() {
	glDeleteBuffers(1, &m_vbo[VERTEX_BUFFER]);
	glDeleteBuffers(1, &m_vbo[TEXCOORD_BUFFER]);
	glDeleteBuffers(1, &m_vbo[NORMAL_BUFFER]);
	glDeleteBuffers(1, &m_vbo[INDEX_BUFFER]);
}

void Mesh::Draw() {
	if (m_material != nullptr)
		if (m_material->GetTexture() != 0)
		App->textures->UseTexture2D(m_material->GetTexture());

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	if (m_material != nullptr)
		glColor4fv(m_material->GetColorComponent(Material::COLOR_COMPONENT::AMBIENT));
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	if (m_material != nullptr)
		glColor4fv(m_material->GetColorComponent(Material::COLOR_COMPONENT::DIFFUSE));
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	if (m_material != nullptr)
		glColor4fv(m_material->GetColorComponent(Material::COLOR_COMPONENT::SPECULAR));
	if (m_material != nullptr)
		glMaterialf(GL_FRONT, GL_SHININESS, m_material->GetShininess());

	if (m_vbo[VERTEX_BUFFER]) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_BUFFER]);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
	}

	if (m_vbo[TEXCOORD_BUFFER]) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TEXCOORD_BUFFER]);
		//glClientActiveTexture(GL_TEXTURE0);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}

	if (m_vbo[NORMAL_BUFFER]) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMAL_BUFFER]);
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, 0, 0);
	}

	if (m_vbo[INDEX_BUFFER]) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[INDEX_BUFFER]);
		glDrawElements(GL_TRIANGLES, m_num_elements, GL_UNSIGNED_INT, NULL);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	App->textures->DontUseTexture2D();
}

