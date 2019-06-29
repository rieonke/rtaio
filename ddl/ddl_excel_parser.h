//
// Created by Rieon Ke on 2019-06-28.
//

#ifndef DDL_GEN_DDL_EXCEL_PARSER_H
#define DDL_GEN_DDL_EXCEL_PARSER_H

#include <vector>
#include "ddl_table.h"
#include "ddl_exception.h"

namespace dh {

    class ddl_excel_parser {

    public:
        std::vector<dh::ddl_table> parse(std::string &file_path);

    };

}

#endif //DDL_GEN_DDL_EXCEL_PARSER_H
