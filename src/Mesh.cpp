#include "Mesh.h"
#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>
#include <vector>
#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Material.h"
#include "Application.h"
#include "ModuleScene.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ProgramManager.h"

Mesh::Mesh()
{}

Mesh::Mesh(aiMesh* mesh, Material* material)
	: m_material(material)
{
	m_num_elements = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		num_vertices = mesh->mNumVertices;
		vertices = new float3[num_vertices];
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i] = float3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		}

		glGenBuffers(1, &m_vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);
	}

	if (mesh->HasTextureCoords(0)) {
		float2* texture_coords = new float2[num_vertices];
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			texture_coords[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}

		glGenBuffers(1, &m_vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(texture_coords[0]), &texture_coords[0], GL_STATIC_DRAW);

		RELEASE_ARRAY(texture_coords);
	}

	if (mesh->HasNormals()) {
		normals = new float3[num_vertices];
		for (size_t i = 0; i < mesh->mNumVertices; ++i) {
			normals[i] = float3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}

		glGenBuffers(1, &m_vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(normals[0]), &normals[0], GL_DYNAMIC_DRAW);
	}

	if (mesh->HasFaces()) {
		GLuint num_indices = mesh->mNumFaces * mesh->mFaces[0].mNumIndices;
		GLuint* indices = new GLuint[num_indices];
		for (size_t i = 0; i < mesh->mNumFaces; ++i) {
			for (size_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices[mesh->mFaces[i].mNumIndices * i + j] = mesh->mFaces[i].mIndices[j];
			}
		}

		glGenBuffers(1, &m_vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		RELEASE_ARRAY(indices);
	}

	if (mesh->HasBones()) {
		vertices_skinned = new float3[num_vertices];
		if (normals != nullptr) {
			normals_skinned = new float3[num_vertices];
		}

		m_num_bones = mesh->mNumBones;
		m_bones = new Bone[m_num_bones];
		// Load each bone
		for (size_t i = 0; i < mesh->mNumBones; ++i) {
			m_bones[i].name = mesh->mBones[i]->mName.C_Str();
			m_bones[i].num_weights = mesh->mBones[i]->mNumWeights;
			m_bones[i].weights = new Weight[m_bones[i].num_weights];

			// Offset matrix (transforms from mesh space to bone space in bind pose)
			for (size_t row = 0; row < 4; ++row) {
				for (size_t col = 0; col < 4; ++col) {
					m_bones[i].bind[row][col] = mesh->mBones[i]->mOffsetMatrix[row][col];
				}
			}

			// For each bone load all weights
			for (size_t j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
				m_bones[i].weights[j].vertex = mesh->mBones[i]->mWeights[j].mVertexId;
				m_bones[i].weights[j].weight = mesh->mBones[i]->mWeights[j].mWeight;
			}
		}
	}

	// Unbind buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Mesh::Mesh(float3 *vertex, unsigned int num_vertices, unsigned int *indices, unsigned int num_indices, float2 *tex_coords, unsigned num_tex_coords)
{
	m_num_elements = num_indices;

	this->num_vertices = num_vertices;
	vertices = new float3[num_vertices];
	for (size_t i = 0; i <num_vertices; ++i) {
		vertices[i] = float3(vertex[i].x, vertex[i].y, vertex[i].z);
	}

	glGenBuffers(1, &m_vbo[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);
	
	//texture coords
	float2* texture_coords = new float2[num_tex_coords];
	for (size_t i = 0; i < num_tex_coords; ++i) {
		texture_coords[i] = tex_coords[i];
	}

	glGenBuffers(1, &m_vbo[TEXCOORD_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[TEXCOORD_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_tex_coords * sizeof(texture_coords[0]), &texture_coords[0], GL_STATIC_DRAW);

	RELEASE_ARRAY(texture_coords);

	// load indices
	GLuint* ind = new GLuint[num_indices];
	for (size_t i = 0; i < num_indices; ++i) {
			ind[i] = indices[i];
	}

	glGenBuffers(1, &m_vbo[INDEX_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(ind[0]), &ind[0], GL_STATIC_DRAW);

	RELEASE_ARRAY(ind);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
	RELEASE_ARRAY(vertices);
	RELEASE_ARRAY(vertices_skinned);
	RELEASE_ARRAY(normals);
	RELEASE_ARRAY(normals_skinned);

	glDeleteBuffers(1, &m_vbo[VERTEX_BUFFER]);
	glDeleteBuffers(1, &m_vbo[TEXCOORD_BUFFER]);
	glDeleteBuffers(1, &m_vbo[NORMAL_BUFFER]);
	glDeleteBuffers(1, &m_vbo[INDEX_BUFFER]);

	for (size_t i = 0; i < m_num_bones; ++i) {
		RELEASE_ARRAY(m_bones[i].weights);
	}

	RELEASE_ARRAY(m_bones);
}

void Mesh::Draw() {

	if (m_material != nullptr)
	{
		if (m_material->GetTexture() != 0)
			App->textures->UseTexture2D(m_material->GetTexture());
		m_material->has_alpha ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		if (m_material->has_shader)
			App->shaders->UseProgram(m_material->shader_name.c_str());
	}

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
	if (m_material->has_shader)
		App->shaders->UnuseProgram();
}

void Mesh::SetMaterial(Material* new_mat)
{
	m_material = new_mat;
}

void Mesh::Init()
{
	// If the mesh is malleable find the corresponding node (game object) for each bone
	for (size_t i = 0; i < m_num_bones; ++i) {
		GameObject* go = App->scene->BoneToGameObjMapping(m_bones[i].name, App->scene->game_objects);
		m_bones[i].attached_to = go;
	}
}

void Mesh::Update()
{

	if (m_num_bones <= 0)
		return;

	float4x4 mat4x4 = float4x4::identity;
	float3x3 mat3x3 = float3x3::identity;
	memset(vertices_skinned, 0, num_vertices * sizeof(float3));
	memset(normals_skinned, 0, num_vertices * sizeof(float3));

	for (size_t b = 0; b < m_num_bones; ++b)
	{
		mat4x4 = m_bones[b].attached_to->GetModelSpaceTransformMatrix() * m_bones[b].bind;
		mat3x3 = mat4x4.Float3x3Part();
		for (size_t w = 0; w < m_bones[b].num_weights; ++w)
		{
			vertices_skinned[m_bones[b].weights[w].vertex] += m_bones[b].weights[w].weight * mat4x4.TransformPos(vertices[m_bones[b].weights[w].vertex]);
			normals_skinned[m_bones[b].weights[w].vertex] += m_bones[b].weights[w].weight * mat3x3.Transform(normals[m_bones[b].weights[w].vertex]);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(vertices_skinned[0]), &vertices_skinned[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(normals_skinned[0]), &normals_skinned[0], GL_DYNAMIC_DRAW);

}
