#ifndef GD_TL_ACTION_H
#define GD_TL_ACTION_H
#include "tl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

gd_tl_event_t gd_tl_event_create(gd_tl_t tl, size_t dataSize);
void * gd_tl_event_data(gd_tl_event_t event);

int gd_tl_event_send(gd_tl_t tl, gd_tl_event_t event);

#ifdef __cplusplus
}
#endif

#endif
