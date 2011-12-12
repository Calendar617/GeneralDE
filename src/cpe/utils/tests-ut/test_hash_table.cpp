#include "HashTest.hpp"

TEST_F(HashTest, insert_basic) {
    TestObject * testObj = createTestObject("a");
    EXPECT_EQ(
        0,
        cpe_hash_table_insert_unique(
            &m_hash_table, testObj));

    EXPECT_TRUE(
        testObj ==
        cpe_hash_table_find(
            &m_hash_table,
            createTestObject("a")));
}

