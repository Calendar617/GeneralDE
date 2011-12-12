#include "HashTest.hpp"

TEST_F(HashTest, insert_unique_basic) {
    TestObject * testObj = createTestObject("a");
    EXPECT_EQ(
        0,
        cpe_hash_table_insert_unique(
            &m_hash_table, testObj));

    EXPECT_EQ(1, cpe_hash_table_count(&m_hash_table));
        
    EXPECT_TRUE(
        testObj ==
        cpe_hash_table_find(
            &m_hash_table,
            createTmpTestObject("a")));
}

TEST_F(HashTest, insert_unique_duplicate) {
    TestObject * testObj = createTestObject("a");
    EXPECT_EQ(
        0,
        cpe_hash_table_insert_unique(
            &m_hash_table, testObj));

    EXPECT_EQ(1, cpe_hash_table_count(&m_hash_table));

    TestObject * testObj2 = createTestObject("a");
    EXPECT_EQ(
        -1,
        cpe_hash_table_insert_unique(
            &m_hash_table, testObj2));
    mem_free(t_allocrator(), testObj2);

    EXPECT_EQ(1, cpe_hash_table_count(&m_hash_table));

    EXPECT_TRUE(
        testObj ==
        cpe_hash_table_find(
            &m_hash_table,
            createTmpTestObject("a")));
}
