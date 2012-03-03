#include "usf/logic/logic_require.h"
#include "usf/logic/logic_require_type.h"
#include "LogicTest.hpp"

class RequireBasicTest : public LogicTest {
public:
    RequireBasicTest()
        : m_require_type(NULL)
        , m_context(NULL)
    {
    }

    void SetUp() {
        LogicTest::SetUp();

        m_context = t_logic_context_create();
        EXPECT_TRUE(m_context);

        m_require_type = logic_require_type_create(t_logic_manager(), "t1");
        EXPECT_TRUE(m_require_type);
    }

    logic_require_t create(const char * name, size_t capacity = 0) {
        return logic_require_create(m_context, cpe_hs_create(t_tmp_allocrator(), name), capacity);
    }

    logic_require_type_t m_require_type;
    logic_context_t m_context;
};

TEST_F(RequireBasicTest, basic) {
    logic_require_t require = create("t1", 123);
    ASSERT_TRUE(require);

    EXPECT_EQ((logic_require_id_t)0, logic_require_id(require));
    EXPECT_EQ(logic_require_state_waiting, logic_require_state(require));
    EXPECT_EQ((size_t)123, logic_require_capacity(require));
    EXPECT_TRUE(logic_require_data(require));

    EXPECT_EQ(logic_context_state_waiting, logic_context_state(m_context));
}

TEST_F(RequireBasicTest, create_no_type) {
    logic_require_t require = create("not-exsit-type");
    EXPECT_TRUE(require == NULL);
}

TEST_F(RequireBasicTest, require_set_done_release) {
    logic_require_t require = create("t1");
    ASSERT_TRUE(require);
    EXPECT_TRUE(require == logic_require_find(t_logic_manager(), 0));

    EXPECT_EQ(logic_require_state_waiting, logic_require_state(require));

    logic_require_set_done(require);

    EXPECT_TRUE(NULL == logic_require_find(t_logic_manager(), 0));

    EXPECT_NE(logic_context_state_waiting, logic_context_state(m_context));
}

TEST_F(RequireBasicTest, require_set_done_keep) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    ASSERT_TRUE(require);
    EXPECT_TRUE(require == logic_require_find(t_logic_manager(), 0));

    EXPECT_EQ(logic_require_state_waiting, logic_require_state(require));

    logic_require_set_done(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_done, logic_require_state(require));

    EXPECT_NE(logic_context_state_waiting, logic_context_state(m_context));
}

TEST_F(RequireBasicTest, require_set_done_from_error) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    logic_require_set_error(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_error, logic_require_state(require));

    logic_require_set_done(require);
    EXPECT_EQ(logic_require_state_done, logic_require_state(require));
}

TEST_F(RequireBasicTest, require_set_done_from_done) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    logic_require_set_done(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_done, logic_require_state(require));

    logic_require_set_done(require);
    EXPECT_EQ(logic_require_state_done, logic_require_state(require));
}

TEST_F(RequireBasicTest, require_set_error_release) {
    logic_require_t require = create("t1");
    ASSERT_TRUE(require);
    EXPECT_TRUE(require == logic_require_find(t_logic_manager(), 0));

    EXPECT_EQ(logic_require_state_waiting, logic_require_state(require));

    logic_require_set_error(require);

    EXPECT_TRUE(NULL == logic_require_find(t_logic_manager(), 0));

    EXPECT_NE(logic_context_state_waiting, logic_context_state(m_context));
}

TEST_F(RequireBasicTest, require_set_error_keep) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    ASSERT_TRUE(require);
    EXPECT_TRUE(require == logic_require_find(t_logic_manager(), 0));

    EXPECT_EQ(logic_require_state_waiting, logic_require_state(require));

    logic_require_set_error(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_error, logic_require_state(require));

    EXPECT_NE(logic_context_state_waiting, logic_context_state(m_context));
}

TEST_F(RequireBasicTest, require_set_error_from_error) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    logic_require_set_error(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_error, logic_require_state(require));

    logic_require_set_error(require);
    EXPECT_EQ(logic_require_state_error, logic_require_state(require));
}

TEST_F(RequireBasicTest, require_set_error_from_done) {
    logic_context_flag_enable(m_context, logic_context_flag_require_keep);

    logic_require_t require = create("t1");
    logic_require_set_done(require);

    ASSERT_TRUE(require == logic_require_find(t_logic_manager(), 0));
    EXPECT_EQ(logic_require_state_done, logic_require_state(require));

    logic_require_set_error(require);
    EXPECT_EQ(logic_require_state_error, logic_require_state(require));
}

