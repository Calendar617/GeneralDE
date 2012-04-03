#include "ManageTest.hpp"

class ManageBasicTest : public ManageTest {
public:
    virtual void SetUp() {
        ManageTest::SetUp();

        setMetaLib(
            "<metalib tagsetversion='1' name='LobbyServer' version='1' >\n"
            "    <struct name='S' version='1'>\n"
            "        <entry name='id' type='uint64'/>\n"
            "        <entry name='attr1' type='int32'/>\n"
            "        <entry name='attr2' type='string' count='4'/>\n"
            "    </struct>\n"
            "</metalib>\n"
            );
    }
};

TEST_F(ManageTest, basic) {
}

