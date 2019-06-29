#include "common.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <xlnt/xlnt.hpp>
#include "ddl/ddl_table.h"
#include "ddl/ddl_table_sql_generator.h"
#include "ddl/ddl_excel_parser.h"
#include <unistd.h>
#include <portable-file-dialogs.h>


int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "");
    bindtextdomain("rtaio_main", getenv("PWD"));
    textdomain("rtaio_main");

    char *home = getenv("HOME");

    std::string path;

    if (argc < 2) {

        pfd::open_file open(_("Choose Excel"), home, {"Xlsx Files)", "*",});

        auto p = open.result();

        if (p.empty()) {
            std::cerr << _("Please choose excel") << std::endl;
            exit(0);
        } else {
            path = p[0];
        }

    } else {
        path = argv[1];
    }

    dh::ddl_excel_parser parser;
    auto tables = parser.parse(path);

    dh::ddl_table_sql_generator generator;
    for (auto t : tables) {
        std::string sql = generator.generate(t);
        std::cout << sql << std::endl;
    }

    return 0;
}