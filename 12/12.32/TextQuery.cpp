#include "TextQuery.h"

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <set>
#include <iostream>

inline std::string make_plural(std::size_t ctr, const std::string &word, const std::string &ending = "s") {
    return (ctr > 1) ? word + ending : word;
}

TextQuery::TextQuery(std::ifstream &is) {
    std::string text;
    while (std::getline(is, text)) {
        file.push_back(text);
        auto n = file.size() - 1;
        std::istringstream line(text);
        std::string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines) { lines.reset(new std::set<line_no>); }
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const std::string &sought) const {
    static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);
    auto loc = wm.find(sought);
    if (loc == wm.end()) {
        return QueryResult(sought, nodata, file);
    } else {
        return QueryResult(sought, loc->second, file);
    }
}

std::ostream &print(std::ostream &os, const QueryResult &qr) {
    os << qr.sought << " occurs " << qr.lines->size() << " " << make_plural(qr.lines->size(), "time", "s") << '\n';
    for (auto num : *qr.lines) {
        auto pos = qr.file.begin();
        for (decltype(num) i = 0; i != num; ++i) { pos.incr(); }
        os << "\t(line " << num + 1 << ") " << pos.deref() << '\n';
    }
    return os;
}
