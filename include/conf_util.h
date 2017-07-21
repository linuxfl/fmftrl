/*************************************************************************
    > File Name: conf_util.h
    > Author: starsnet83
    > Mail: starsnet83@gmail.com 
    > Created Time: 四 12/18 15:05:32 2014
 ************************************************************************/
#ifndef CONF_UTIL_H
#define CONF_UTIL_H

#include <string>
#include <map>
#include <sstream> 

namespace util {
    class conf_util {
    public:
        void parse(const std::string &conf_file);

        bool has_item(const std::string &item_name);

        template<class T>
        T getItem(const std::string item_name) {
            std::stringstream stream;
            T result;
            stream << conf_items[item_name];
            stream >> result;
            return result;
        }

    private:
        std::map<std::string, std::string> conf_items;
    };
}
#endif
