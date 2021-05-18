#include "world.hpp"

entityHandle_t World::AddEntity(const char* name) 
{
    entityHandle_t handle;
    handle.id   = _entities_top + 1;
    handle.slot = _entities_top;
    handle.name = name;

    this->_entity_handles[this->_entities_top] = handle;
    this->_entities_top += 1;

    // Add a spatial component by default because how can an entity exist in the world if it's not IN the world?
    this->entity_transforms.Add(handle);
    // this->UpdateHandle(handle);

    return handle;
}

entityHandle_t World::AddChildEntity(entityHandle_t parent)
{
    entityHandle_t handle = this->AddEntity(parent.name.c_str());
    handle.parent_id = parent.id;

    // Clone the spatial data from the parent
    this->entity_transforms.positions[handle.slot] = this->entity_transforms.positions[parent.slot];
    this->entity_transforms.rotations[handle.slot] = this->entity_transforms.rotations[parent.slot];
    this->entity_transforms.scales[handle.slot] = this->entity_transforms.scales[parent.slot];

     parent.num_children += 1;

    return handle;
}

//std::vector<entityHandle_t> World::GetChildren(entityHandle_t parent)
//{
//    std::vector<entityHandle_t> children;
//    for (int i = 0; i < parent._child_entities_top; i++) {
//        for (int j = 0; j < this->_entities_top; j++) {
//            if (this->_entity_handles[j].id == i) {
//                children.push_back(this->_entity_handles[j]);
//            }
//        }
//    }
//
//    return children;
//}

materialHandle_t World::AddMaterial(RenderMaterial material)
{
    for (int i = 0; i < this->_materials_top; i++) {
        if (this->_material_handles[i].name == material.name) {
            return this->_material_handles[i];
        }
    }
    materialHandle_t handle;
    handle.id = this->_materials_top + 1;
    handle.slot = this->_materials_top;
    handle.name = material.name;

    this->_materials[this->_materials_top] = material;
    this->_material_handles[this->_materials_top] = handle;
    this->_materials_top += 1;

    return handle;
}

entityHandle_t World::GetHandle(const char* name)
{
    for (int i = 0; i < this->_entities_top; i++) {
        if (this->_entity_handles[i].name == name) {
            return this->_entity_handles[i];
        }
    }
}