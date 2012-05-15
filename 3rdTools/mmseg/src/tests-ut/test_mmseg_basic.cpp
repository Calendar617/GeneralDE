#include <string>
#include "MMSegTest.hpp"

TEST_F(MMSegTest, basic) {
    css::Segmenter* seg = m_segmentManage.getSegmenter();
    ASSERT_TRUE(seg);

    char* tok;
	u2 len = 0, symlen = 0;

    char input[] = "中文分词测试";
    seg->setBuffer((u1*)input, sizeof(input));

	tok = (char*)seg->peekToken(len, symlen);
    ASSERT_TRUE(tok);
    EXPECT_STREQ("中文", ::std::string(tok, symlen).c_str());
	EXPECT_TRUE(!seg->isSentenceEnd());
    seg->popToken(len, symlen);

	tok = (char*)seg->peekToken(len, symlen);
    ASSERT_TRUE(tok);
    EXPECT_STREQ("分", ::std::string(tok, symlen).c_str());
	EXPECT_TRUE(!seg->isSentenceEnd());
    seg->popToken(len, symlen);

	tok = (char*)seg->peekToken(len, symlen);
    ASSERT_TRUE(tok);
    EXPECT_STREQ("词", ::std::string(tok, symlen).c_str());
	EXPECT_TRUE(!seg->isSentenceEnd());
    seg->popToken(len, symlen);

	tok = (char*)seg->peekToken(len, symlen);
    ASSERT_TRUE(tok);
    EXPECT_STREQ("测试", ::std::string(tok, symlen).c_str());
	EXPECT_TRUE(!seg->isSentenceEnd());
    seg->popToken(len, symlen);

	tok = (char*)seg->peekToken(len, symlen);
    ASSERT_TRUE(tok == NULL);
	EXPECT_TRUE(seg->isSentenceEnd());
}
