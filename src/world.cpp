#include "world.hpp"

void World::AddEntity(entityHandle_t entity)
{
    this->_entity_handles[this->_entities_top] = entity;
    this->_entities_top += 1;
}