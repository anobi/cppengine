#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <memory>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "opengl.hpp"
#include "entitycomponent.hpp"
#include "mesh.hpp"

class Model : public EntityComponent
{
public:
	Model(const std::string fileName);
	~Model();
	std::vector<std::shared_ptr<Mesh>> meshes;
	void Render(Renderer& renderer);
private:
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh);
	void ProcessNode(aiNode* node, const aiScene* scene);
};

#endif
