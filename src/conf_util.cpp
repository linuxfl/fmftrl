/*************************************************************************
    > File Name: conf_util.cpp
    > Author: starsnet83
    > Mail: starsnet83@gmail.com 
    > Created Time: 四 12/18 15:05:32 2014
 ************************************************************************/
#include <fstream>
#include "conf_util.h"
#include "str_util.h"

namespace util {
    using namespace std;

    void conf_util::parse(const string &conf_file) {
        ifstream stream(conf_file.c_str(), ifstream::in);
        string line;
        vector <string> fields;
        while (getline(stream, line)) {
            fields.clear();
            str_util::trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            str_util::split(line, "=", fields);
            str_util::trim(fields[0]);
            str_util::trim(fields[1]);

            conf_items[fields[0]] = fields[1];
        }
        stream.clear();
        stream.close();
    }

    bool conf_util::has_item(const string &item_name) {
        if (conf_items.find(item_name) != conf_items.end()) {
            return true;
        }
        return false;
    }
}
