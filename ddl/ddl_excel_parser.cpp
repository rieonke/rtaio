//
// Created by Rieon Ke on 2019-06-28.
//

#include "common.h"
#include "ddl_excel_parser.h"
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <xlnt/xlnt.hpp>

#define CELL_TABLE_NAME "B2"
#define CELL_TABLE_SPACE "B3"
#define CELL_TABLE_DESCRIPTION "B4"
#define CELL_TABLE_SCHEMA "B5"

#define COL_START_ROW 8
#define COL_END_COL 13


std::vector<dh::ddl_table> dh::ddl_excel_parser::parse(std::string &path) {

    char *home = getenv("HOME");

    if (path[0] == '~') {
        path.erase(0, 1);
        path = home + path;
    }

    unsigned long pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        throw ddl_exception(_("File name must end with .xlsx"));
//        std::cerr << "请选择xlsx结尾的Excel文件" << std::endl;
//        exit(-2);
    }

    auto ext = path.substr(pos);
    if (!boost::iequals(ext, ".xlsx")) {
        throw ddl_exception(_("File name must end with .xlsx"));
    }

    struct stat buffer;
    if (stat(path.c_str(), &buffer) != 0) {
        throw ddl_exception(_("File does not exists"));
    }


    xlnt::workbook wb;
    wb.load(path);

//1. check sheets count
    size_t sheet_count = wb.sheet_count();

    if (sheet_count <= 0) {
        throw ddl_exception(_("Excel workbook is empty"));
//        std::cerr << "empty workbook, exit" << std::endl;
//        exit(1);
    }

//2. get all the sheets


    std::vector<dh::ddl_table> tables;
    for (int i = 0; i < sheet_count; i++) {
        const xlnt::worksheet &worksheet = wb.sheet_by_index(i);

//if current sheet does not have cell A1,skip
        if (!worksheet.has_cell("A1")) {
            continue;
        }

//if cell A1 is not 'BEGIN' , skip
        const xlnt::cell &cell = worksheet.cell("A1");
        const std::string &cv = cell.to_string();
        if (cv != "BEGIN") {
            continue;
        }


        if (!worksheet.has_cell(CELL_TABLE_NAME)) {
            continue;
        }

        dh::ddl_table table;

        auto c_table_name = worksheet.cell(CELL_TABLE_NAME); //name
        table.name = c_table_name.to_string();

        if (worksheet.has_cell(CELL_TABLE_SPACE)) {
            auto c_table_space = worksheet.cell(CELL_TABLE_SPACE); //tablespace
            const std::string &str = c_table_space.to_string();
            table.tablespace = str;
        }

        if (worksheet.has_cell(CELL_TABLE_DESCRIPTION)) {
            auto c_description = worksheet.cell(CELL_TABLE_DESCRIPTION); //description
            table.description = c_description.to_string();
        }

        if (worksheet.has_cell(CELL_TABLE_SCHEMA)) {
            auto c_description = worksheet.cell(CELL_TABLE_SCHEMA); //description
            table.schema = c_description.to_string();
        }

        for (auto row : worksheet.rows(true)) {

            bool skip = false;
            dh::ddl_table_column t_col;

            for (auto cell : row) {
                int col = cell.column_index();
                if (cell.row() < COL_START_ROW || col > COL_END_COL) {
                    skip = true;
                    continue;
                }

                std::string val = cell.to_string();
                boost::trim(val);

                if (val == "END") {
                    goto end_row_loop;
                }

                switch (col) {
                    case 1:
                        t_col.order = std::stoi(val);
                        break;
                    case 2:
                        t_col.name = val;
                        break;

                    case 3:
                        t_col.type = val;
                        break;

                    case 4:
                        t_col.not_null = val == "TRUE";
                        break;
                    case 5:
                        t_col.auto_increment = (val == "TRUE");
                        break;

                    case 6:
                        t_col.default_value = val;
                        break;

                    case 7:
                        t_col.is_primary_key = !val.empty();

                        if (t_col.is_primary_key && !table.primary_key.empty() && val != table.primary_key) {
                            throw ddl_exception(table.name + " " + _("Primary key value must be unique"));
                        }

                        if (t_col.is_primary_key && table.primary_key.empty()) {
                            table.primary_key = val;
                        }

                        break;
                    case 8:
                        t_col.description = val;
                        break;
                    case 9:
                        t_col.comment = val;
                        break;
                    case 10:
                        t_col.generated_value = val;
                        break;
                    case 11:
                        t_col.foreign_key = val;
                        break;
                    case 12:
                        t_col.ref_foreign_table = val;
                        break;
                    case 13:
                        t_col.ref_foreign_field = val;
                        break;
                }

            }
            if (!skip)
                table.columns.push_back(t_col);
        }

        end_row_loop:

        tables.push_back(table);
    }

    return tables;

}

