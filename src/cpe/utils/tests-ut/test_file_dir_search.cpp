#include <sstream>
#include "SearchDirTest.hpp"

TEST_F(SearchDirTest, file_go) {
    expectFile("a.txt", dir_visit_next_go);
    t_write_to_file("a.txt", "");
    do_search(-1);
    CHECK_SEARCHDIR_RESULT();
}
