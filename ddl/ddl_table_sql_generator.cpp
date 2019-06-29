//
// Created by Rieon Ke on 2019-06-27.
//

#include "ddl_table_sql_generator.h"

#define SQL_SPACE " "
#define SQL_BR "\n"
#define SQL_CLM ","
#define SQL_DLM ";"


std::string dh::ddl_table_sql_generator::generate(const dh::ddl_table &table) {

    std::string sql;

    std::string table_name = table.schema + "." + table.name;
    std::string primary_keys;

    sql.append("CREATE TABLE ").append(table_name).append("(").append(SQL_BR);


    for (int i = 0; i < table.columns.size(); i++) {

        auto col = table.columns[i];

        if (col.is_primary_key) {
            primary_keys.append(col.name).append(",");
        }

        sql.append(SQL_SPACE).append(col.name).append(SQL_SPACE).append(col.type).append(SQL_SPACE);

        if (!col.default_value.empty()) {
            sql.append("DEFAULT ").append(col.default_value).append(SQL_SPACE);
        }

        if (col.auto_increment) {
            sql.append("GENERATED AS IDENTITY ");
        }

        if (!col.auto_increment && !col.generated_value.empty()) {
            sql.append("GENERATED ALWAYS AS (").append(col.generated_value).append(")").append(SQL_SPACE);
        }

        if (col.not_null) {
            sql.append("NOT NULL").append(SQL_SPACE);
        }

        if (i != table.columns.size() - 1) {
            sql.append(SQL_CLM);
        }

        sql.append(SQL_BR);
    }


    sql.append(")");
    if (!table.tablespace.empty()) {
        sql.append("TABLESPACE").append(SQL_SPACE).append(table.tablespace);
    }

    sql.append(SQL_DLM).append(SQL_BR);


    if (!table.description.empty()) {

        //comment on table T_TEST_TABLE is 'dasdasda';
        sql.append("COMMENT ON TABLE ").append(table_name).append(SQL_SPACE);
        sql.append("IS '").append(table.description).append("'");

        sql.append(SQL_DLM).append(SQL_BR);
    }


    for (int j = 0; j < table.columns.size(); j++) {

        auto col = table.columns[j];
        if (col.description.empty() && col.comment.empty()) {
            continue;
        }

        sql.append("COMMENT ON COLUMN ");

        sql.append(table_name).append(".");
        sql.append(col.name).append(SQL_SPACE);
        sql.append("IS").append(SQL_SPACE);

        sql.append("'").append(col.description).append(",").append(col.comment).append("'");

        sql.append(SQL_DLM).append(SQL_BR);
    }

    if (!table.primary_key.empty()) {

        //alter table hr.T_TEST_TABLE add constraint T_TEST_TABLE_PK primary key ();

        sql.append("ALTER TABLE ").append(table_name).append(SQL_SPACE);
        sql.append("ADD CONSTRAINT ").append(table.primary_key).append(SQL_SPACE);

        sql.append("PRIMARY KEY (");

        if (primary_keys.find_last_of(',') == primary_keys.size() - 1) {
            primary_keys.erase(primary_keys.size() - 1);
        }

        sql.append(primary_keys);

        sql.append(")");

        sql.append(SQL_DLM).append(SQL_BR);
    }


    for (int j = 0; j < table.columns.size(); j++) {

        auto col = table.columns[j];

        if (col.foreign_key.empty()) {
            continue;
        }
        //alter table T_TEST_TABLE add constraint T_TEST_FK foreign key (id) REFERENCES REGIONS(REGION_ID);

        sql.append("ALTER TABLE ");
        sql.append(table_name).append(SQL_SPACE);
        sql.append("ADD CONSTRAINT ").append(col.foreign_key).append(SQL_SPACE);
        sql.append("FOREIGN KEY (").append(col.name).append(")").append(SQL_SPACE);
        sql.append("REFERENCES ").append(col.ref_foreign_table).append("(").append(col.ref_foreign_field).append(")");
        sql.append(SQL_DLM).append(SQL_BR);
    }

    return sql;
}
