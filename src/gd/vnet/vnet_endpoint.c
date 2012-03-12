#include <assert.h>
#include "cpe/pal/pal_external.h"
#include "gd/nm/nm_manage.h"
#include "gd/nm/nm_read.h"
#include "gd/app/app_context.h"
#include "gd/app/app_module.h"
#include "gd/vnet/vnet_manage.h"
#include "gd/vnet/vnet_endpoint.h"
#include "vnet_internal_ops.h"

static void vnet_ep_clear(gd_nm_node_t node);

struct gd_nm_node_type s_nm_node_type_vnet_endpoint = {
    "gd_vnet_endpoint",
    vnet_ep_clear
};

vnet_ep_t
vnet_ep_create(
    vnet_manage_t mgr, const char * name, size_t capacity)
{
    vnet_ep_t ep;
    gd_nm_node_t ep_node;

    ep_node = gd_nm_group_create(gd_app_nm_mgr(mgr->m_app), name, sizeof(struct vnet_ep));
    if (ep_node == NULL) return NULL;

    ep = (vnet_ep_t)gd_nm_node_data(ep_node);
    ep->m_mgr = mgr;
    ep->m_capacity = capacity;
    ep->m_dispatcher_fun = NULL;
    ep->m_dispatcher_ctx = NULL;

    gd_nm_node_set_type(ep_node, &s_nm_node_type_vnet_endpoint);

    return ep;
}

static void vnet_ep_clear(gd_nm_node_t node) {
    vnet_ep_t ep;
    ep = (vnet_ep_t)gd_nm_node_data(node);
}

void vnet_ep_free(vnet_ep_t ep) {
    gd_nm_node_t ep_node;
    assert(ep);

    ep_node = gd_nm_node_from_data(ep);
    if (gd_nm_node_type(ep_node) != &s_nm_node_type_vnet_endpoint) return;
    gd_nm_node_free(ep_node);
}

vnet_ep_t
vnet_ep_find(
    vnet_manage_t mgr,
    cpe_hash_string_t name)
{
    gd_nm_node_t node = gd_nm_mgr_find_node(gd_app_nm_mgr(mgr->m_app), name);
    if (node == NULL || gd_nm_node_type(node) != &s_nm_node_type_vnet_endpoint) return NULL;
    return (vnet_ep_t)gd_nm_node_data(node);
}

const char * vnet_ep_name(vnet_ep_t ep) {
    return gd_nm_node_name(gd_nm_node_from_data(ep));
}

cpe_hash_string_t
vnet_ep_name_hs(vnet_ep_t ep) {
    return gd_nm_node_name_hs(gd_nm_node_from_data(ep));
}

size_t vnet_ep_capacity(vnet_ep_t ep) {
    return ep->m_capacity;
}

void * vnet_ep_data(vnet_ep_t ep) {
    return ep + 1;
}

void vnet_ep_set_dispatcher(
    vnet_ep_t ep,
    vnet_dispatch_fun_t fun,
    void * ctx)
{
    ep->m_dispatcher_fun = fun;
    ep->m_dispatcher_ctx = ctx;
}
