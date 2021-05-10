#ifndef __ENTITIES__ENTITY_HANDLE_H__
#define __ENTITIES__ENTITY_HANDLE_H__

constexpr int INVALID_HANDLE_ID = 0;
constexpr int INVALID_SLOT = -1;

struct entityHandle_t {
    char name[64]       = "Entity";
    unsigned int id     = INVALID_HANDLE_ID;

    int slot            = INVALID_SLOT;
    int spatial_component_slot = INVALID_SLOT;
    int render_component_slot  = INVALID_SLOT;
    int light_component_slot   = INVALID_SLOT;

    bool valid() {
        return (id != INVALID_HANDLE_ID && slot != INVALID_SLOT);
    };
};

#endif  // __ENTITIES__ENTITY_HANDLE_H__