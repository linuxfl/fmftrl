/*************************************************************************
    > File Name: str_util.h
    > Author: starsnet83
    > Mail: starsnet83@gmail.com 
    > Created Time: 三 12/17 00:13:30 2014
 ************************************************************************/
#include <vector>
#include "str_util.h"
#include <iostream>

namespace util {
    using namespace std;

    void str_util::split(const std::string &line, const std::string delim,
                         std::vector<std::string> &elems, bool hasSpace) {
        size_t pos = 0;
        size_t len = line.length();
        size_t delim_len = delim.length();

        if (delim_len == 0) {
            return;
        }
        //没有查找完
        while (pos < len) {
            size_t find_pos = line.find(delim, pos);
            //查找完成
            if (find_pos == string::npos) {
                elems.push_back(line.substr(pos));
                break;
            }
            //没有查找完成，且有字符
            if (find_pos != pos) {
                elems.push_back(line.substr(pos, find_pos - pos));
            }
            else//没有字符
            {
                if (hasSpace) {
                    elems.push_back("");
                }
            }
            pos = find_pos + delim_len;
        }
    }

    std::vector<std::string> str_util::split(const std::string &line, const std::string delim, bool hasSpace) {
        std::vector<std::string> elems;
        size_t pos = 0;
        size_t len = line.length();
        size_t delim_len = delim.length();

        if (delim_len == 0) return elems;
        //没有查找完
        while (pos < len) {
            size_t find_pos = line.find(delim, pos);
            //查找完成
            if (find_pos == string::npos) {
                elems.push_back(line.substr(pos));
                break;
            }
            //没有查找完成，且有字符
            if (find_pos != pos) {
                elems.push_back(line.substr(pos, find_pos - pos));
            }
            else//没有字符
            {
                if (hasSpace) {
                    elems.push_back("");
                }
            }
            pos = find_pos + delim_len;
        }

        return elems;
    }

    void str_util::trim(std::string &line) {
        if (line.empty()) {
            return;
        }

        line.erase(0, line.find_first_not_of(" "));
        line.erase(line.find_last_not_of(" ") + 1);
    }

    void str_util::rtrim(std::string &line) {
        if (line.empty()) {
            return;
        }
        line.erase(line.find_last_not_of(" ") + 1);
    }

    void str_util::ltrim(std::string &line) {
        if (line.empty()) {
            return;
        }
        line.erase(0, line.find_first_not_of(" "));
    }
}
