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
#include "shader.hpp"
#include "material.hpp"

class Model : public EntityComponent
{
public:
	Model(const std::string fileName);
	~Model();

	void Update() {};
	void Render(Renderer &renderer) {};
	void Render(std::shared_ptr<Shader> shader);
	
private:

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;

	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
};

#endif
