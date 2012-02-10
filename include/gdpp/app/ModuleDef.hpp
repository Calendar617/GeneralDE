#ifndef GDPP_APP_MODULE_DEF_H
#define GDPP_APP_MODULE_DEF_H

#define GDPP_APP_MODULE_DEF(__module_name, __module_impl)               \
    static cpe_hash_string_buf s_ ## __module_name ## _Name =           \
        CPE_HS_BUF_MAKE(#__module_name);                                \
                                                                        \
    cpe_hash_string_t                                                   \
    __module_name::NAME = (cpe_hash_string_t)                           \
        &s_ ## __module_name ## _Name;                                  \
                                                                        \
    extern "C"                                                          \
    int __module_name ## _app_init(                                     \
        Gd::App::Application & app,                                     \
        Gd::App::Module & module,                                       \
        Cpe::Cfg::Node & moduleCfg)                                     \
    {                                                                   \
        try {                                                           \
            new (app.nmManager(), cpe_hs_data(__module_name::NAME))     \
                __module_impl(app, moduleCfg);                          \
            return 0;                                                   \
        }                                                               \
        APP_CTX_CATCH_EXCEPTION(app, #__module_name " init:");          \
        return -1;                                                      \
    }                                                                   \
                                                                        \
    extern "C"                                                          \
    void __module_name ## _app_fini(                                    \
        Gd::App::Application & app,                                     \
        Gd::App::Module & module)                                       \
    {                                                                   \
        app.nmManager().removeObject(__module_name::NAME);              \
    }                                                                   \


#endif
