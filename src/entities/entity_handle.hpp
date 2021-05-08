#ifndef __ENTITIES__ENTITY_HANDLE_H__
#define __ENTITIES__ENTITY_HANDLE_H__

constexpr int INVALID_HANDLE_ID = 0;
constexpr int INVALID_SLOT = -1;

struct entityHandle_T {
    unsigned int id = INVALID_HANDLE_ID;
    unsigned int slot = INVALID_SLOT;
};

#endif  // __ENTITIES__ENTITY_HANDLE_H__