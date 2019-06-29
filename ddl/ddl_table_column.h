//
// Created by Rieon Ke on 2019-06-27.
//

#ifndef DDLHELPER_DDL_TABLE_COLUMN_H
#define DDLHELPER_DDL_TABLE_COLUMN_H

#include <string>

namespace dh {

    class ddl_table_column {

    public:
        int order;
        std::string name;
        std::string type;
        bool not_null = false;
        bool auto_increment = false;
        std::string default_value;
        bool is_primary_key = false;
        std::string description;
        std::string comment;
        std::string generated_value;
        std::string foreign_key;
        std::string ref_foreign_table;
        std::string ref_foreign_field;

    };
}


#endif //DDLHELPER_DDL_TABLE_COLUMN_H
