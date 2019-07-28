//
// Created by Rieon Ke on 2019-06-28.
//

#ifndef DDL_GEN_DDL_EXCEPTION_H
#define DDL_GEN_DDL_EXCEPTION_H

#include <exception>
#include <string>

namespace dh {

    class ddl_exception : public std::exception {

    public:
        ddl_exception(const std::string &what) {
            _what = what;
        }

        const char *what() {
            return _what.c_str();
        }

    private:
        std::string _what;


    };

}

#endif //DDL_GEN_DDL_EXCEPTION_H
