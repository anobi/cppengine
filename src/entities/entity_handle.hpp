#ifndef __ENTITIES__ENTITY_HANDLE_H__
#define __ENTITIES__ENTITY_HANDLE_H__

constexpr int INVALID_HANDLE_ID = 0;
constexpr int INVALID_SLOT = -1;

struct entityHandle_t {
    unsigned int id = INVALID_HANDLE_ID;
    unsigned int slot = INVALID_SLOT;

    unsigned int spatial_component_slot = INVALID_SLOT;
    unsigned int render_component_slot = INVALID_SLOT;
    unsigned int light_component_slot = INVALID_SLOT;

    bool valid() {
        return (id != INVALID_HANDLE_ID && slot != INVALID_SLOT);
    };
};

#endif  // __ENTITIES__ENTITY_HANDLE_H__