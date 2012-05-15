#include "MMSegTest.hpp"

TEST_F(MMSegTest, basic) {
    css::Segmenter* seg = m_segmentManage.getSegmenter();
    ASSERT_TRUE(seg);

    char input[] = "今天天气真好,我们干什么去呢";
	u2 len = 0, symlen = 0;
    seg->setBuffer((u1*)input, sizeof(input));

	char* tok = (char*)seg->peekToken(len, symlen);
    printf("%s len=%d\n", tok, symlen);
    //segment(out_file,seg);
}
