#include "world.hpp"

entityHandle_T World::AddEntity(Entity* entity) 
{
    entityHandle_T handle;
    handle.id   = _entities_top + 1;
    handle.slot = _entities_top;

    this->_entities[this->_entities_top] = entity;
    this->_entities_top += 1;

    return handle;
}

Entity* World::GetEntity(entityHandle_T handle) 
{
    if (handle.id != INVALID_HANDLE_ID && handle.id <= this->_entities_top - 1) {
        return this->_entities[handle.id];
    }
    return 0;
}