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

