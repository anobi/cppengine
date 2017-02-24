#include "model.hpp"

Model::Model(const std::string fileName) : EntityComponent() {

	this->SetName("Mesh");

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(fileName,
		aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_GenSmoothNormals
		| aiProcess_JoinIdenticalVertices
		| aiProcess_FlipUVs
		| aiProcess_FixInfacingNormals
		| aiProcess_CalcTangentSpace);

	if (scene == NULL) return;

	this->ProcessNode(scene->mRootNode, scene);
}

Model::~Model()
{

}

void Model::Render(Renderer &renderer) 
{
	for (int i = 0; i < this->meshes.size(); i++) 
	{
		this->meshes[i]->Draw();
	}
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		auto mesh = this->ProcessMesh(aiMesh, scene);
		meshes.push_back(mesh);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//construct the vertexes
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//positions
		if (mesh->HasPositions())
		{
			aiVector3D vert = mesh->mVertices[i];
			vertex.position = glm::fvec3(vert.x, vert.y, vert.z);
		}

		//normals
		if (mesh->HasNormals())
		{
			aiVector3D norm = mesh->mNormals[i];
			vertex.normal = glm::fvec3(norm.x, norm.y, norm.z);
		}

		//normals
		if (mesh->HasTangentsAndBitangents())
		{
			aiVector3D t = mesh->mTangents[i];
			aiVector3D b = mesh->mBitangents[i];
			vertex.tangent = glm::fvec3(t.x, t.y, t.z);
			vertex.bitangent = glm::fvec3(b.x, b.y, b.z);
		}

		if (mesh->mTextureCoords[0])
		{
			//texture coordinates
			auto uv = mesh->mTextureCoords[0][i];
			vertex.texCoords = glm::fvec2(uv.x, uv.y);
		}
		else
		{
			vertex.texCoords = glm::fvec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//indices
	if (mesh->HasFaces()) 
	{
		for (unsigned int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace &face = mesh->mFaces[j];

			for (unsigned int k = 0; k < face.mNumIndices; k++)
			{
				indices.push_back(face.mIndices[k]);
			}
		}
	}

	if (mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];
		std::shared_ptr<Material> material = std::make_shared<Material>();

		material->LoadMaps(aiMat);
		materials.push_back(material);
	}

	auto m = std::make_shared<Mesh>(Mesh(vertices, indices));
	return m;
}