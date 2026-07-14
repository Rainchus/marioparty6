#include "gssdk/safeh.h"

#include <stddef.h>

#define RSRC_TYPE 0x1E61
#define RSRC_INVALID_RESOURCE 0x80160101
#define RSRC_OUT_OF_MEMORY 0x80160186

typedef struct Resource {
    u16 type;
    u16 reserved;
    void *heap;
    void *value0;
    void *value1;
    void *value2;
} Resource;

extern u32 osspi_IsGoodWritePtr(void *address, u32 size);
extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

const Resource g_NullResource = { RSRC_TYPE };

static inline BOOL IsError(u32 result)
{
    return result >= 0x80000000 ? TRUE : FALSE;
}

u32 InitRsrcFunction(SafeHandle handle, void **resource)
{
    u32 result = 0;

    if (safeh_HandlesEqual(handle, g_lhNullHandle)) {
        *resource = (void *)&g_NullResource;
    } else {
        result = safeh_HandleCheck(handle, RSRC_TYPE, sizeof(Resource));
        if (IsError(result)) {
            if ((result & 0x7F) == 9) {
                result = RSRC_INVALID_RESOURCE;
            }
            return result;
        }
        *resource = handle.address;
        if (*resource == NULL || ((Resource *)*resource)->type != RSRC_TYPE) {
            return RSRC_INVALID_RESOURCE;
        }
    }
    return result;
}

u32 rsrc_Open(void *heap, void *value0, void *value1, void *value2, SafeHandle *handle)
{
    Resource *resource = NULL;
    u32 result;

    result = osspi_IsGoodWritePtr(handle, sizeof(SafeHandle));
    if (!IsError(result)) {
        resource = heap_Calloc(heap, sizeof(Resource), 1);
        if (resource == NULL) {
            result = RSRC_OUT_OF_MEMORY;
        } else {
            resource->heap = heap;
            resource->value1 = value1;
            resource->value0 = value0;
            resource->value2 = value2;
            handle->address = resource;
            handle->type = RSRC_TYPE;
            resource->heap = heap;
            resource->value1 = value1;
            resource->value0 = value0;
            resource->value2 = value2;
            resource->type = RSRC_TYPE;
        }
    }
    if (IsError(result) && resource != NULL) {
        resource->type = 0;
        heap_Free(resource->heap, resource);
    }
    return result;
}

u32 rsrc_Close(SafeHandle handle)
{
    u32 result;
    Resource *resource;
    Resource *closeResource;

    result = InitRsrcFunction(handle, (void **)&resource);
    if (!IsError(result) && (closeResource = resource) != NULL) {
        closeResource->type = 0;
        heap_Free(closeResource->heap, closeResource);
    }
    return result;
}
