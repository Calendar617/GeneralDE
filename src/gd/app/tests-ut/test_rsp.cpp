#include "gd/dp/dp_responser.h"
#include "gd/dp/dp_manage.h"
#include "AppTest.hpp"

class RspTest : public AppTest {
public:
    virtual void SetUp() {
        AppTest::SetUp();
        installTestModule();
    }
};

TEST_F(RspTest, rsp_load_basic) {
    t_em_set_print();

    EXPECT_EQ(
        0,
        t_app_install_rsps(
            "test_module",
            "rsps:\n"
            "  - name: rsp1\n"
            "    rsp-to: [1 ack]\n"
            "    processor: rsp_processor_1\n"
            ));

    gd_dp_rsp_t rsp1 = 
        gd_dp_rsp_find_by_numeric(t_dp(), 1);
    ASSERT_TRUE(rsp1);
    EXPECT_STREQ("rsp1", gd_dp_rsp_name(rsp1));
}

