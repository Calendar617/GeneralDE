#ifndef GD_NM_TYPES_H
#define GD_NM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gd_nm_node_type {
    gd_nm_node_group,
    gd_nm_node_instance
} gd_nm_node_type_t;

typedef struct gd_nm_mgr * gd_nm_mgr_t;
typedef struct gd_nm_node * gd_nm_node_t;

#ifdef __cplusplus
}
#endif

#endif
