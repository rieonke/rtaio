//
// Created by Rieon Ke on 2019-06-27.
//

#ifndef DDLHELPER_DDL_TABLE_H
#define DDLHELPER_DDL_TABLE_H

#include <string>
#include <vector>
#include "ddl_table_column.h"

namespace dh {


    class ddl_table {

    public:
        std::string schema;
        std::string name;
        std::string description;
        std::string tablespace;
        std::string primary_key;

        std::vector<dh::ddl_table_column> columns;
    };

}

#endif //DDLHELPER_DDL_TABLE_H
