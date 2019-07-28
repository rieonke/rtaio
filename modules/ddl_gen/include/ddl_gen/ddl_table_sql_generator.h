//
// Created by Rieon Ke on 2019-06-27.
//

#ifndef DDLHELPER_DDL_TABLE_SQL_GENERATOR_H
#define DDLHELPER_DDL_TABLE_SQL_GENERATOR_H

#include <string>
#include "ddl_table.h"

namespace dh {

    class ddl_table_sql_generator {

    public:
        std::string generate(const ddl_table &table);

    };

}

#endif //DDLHELPER_DDL_TABLE_SQL_GENERATOR_H
