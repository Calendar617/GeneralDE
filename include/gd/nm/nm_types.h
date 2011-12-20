#ifndef GD_NM_TYPES_H
#define GD_NM_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum gd_nm_node_category {
    gd_nm_node_group,
    gd_nm_node_instance
} gd_nm_node_category_t;

typedef struct gd_nm_mgr * gd_nm_mgr_t;
typedef struct gd_nm_node * gd_nm_node_t;
typedef struct gd_nm_node_it * gd_nm_node_it_t;

typedef gd_nm_node_t (*gd_nm_it_next_fun)(gd_nm_node_it_t it);

struct gd_nm_node_it {
    gd_nm_it_next_fun m_next_fun;
    char m_reserve[32];
};

#ifdef __cplusplus
}
#endif

#endif
