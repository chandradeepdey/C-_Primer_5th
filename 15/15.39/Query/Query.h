#ifndef C_PRIMER_5TH_QUERY_H
#define C_PRIMER_5TH_QUERY_H

#include "TextQuery.h"
#include <string>
#include <utility>
#include <memory>
#include <iostream>
#include <set>
#include <algorithm>
#include <iterator>

class Query_base {
    friend class Query;

protected:
    using line_no = TextQuery::line_no;

    Query_base() = default;

    Query_base(const Query_base &other) = default;

    Query_base &operator=(const Query_base &other) = default;

    Query_base(Query_base &&other) noexcept = default;

    Query_base &operator=(Query_base &&other) noexcept = default;

    virtual ~Query_base() = default;

private:
    virtual QueryResult eval(const TextQuery &t) const = 0;

    virtual std::string what() const = 0;
};


class WordQuery : public Query_base {
    friend class Query;

private:
    WordQuery(std::string s) : query_word(std::move(s)) {}

    QueryResult eval(const TextQuery &t) const override { return t.query(query_word); }

    std::string what() const override { return query_word; }

private:
    std::string query_word;
};

class Query {
    friend Query operator~(Query operand);

    friend Query operator|(Query lhs, Query rhs);

    friend Query operator&(Query lhs, Query rhs);

public:
    Query(std::string s) : q(new WordQuery(std::move(s))) {}

    QueryResult eval(const TextQuery &t) const { return q->eval(t); }

    std::string what() const { return q->what(); }

private:
    Query(std::shared_ptr<Query_base> query) : q(std::move(query)) {}

    std::shared_ptr<Query_base> q;
};

inline std::ostream &operator<<(std::ostream &os, const Query &query) { return os << query.what(); }

class NotQuery : public Query_base {
    friend Query operator~(Query operand);

private:
    NotQuery(Query q) : query(std::move(q)) {}

    QueryResult eval(const TextQuery &t) const override;

    std::string what() const override { return "~ ( " + query.what() + " )"; }

private:
    Query query;
};

inline Query operator~(Query operand) {
    return std::shared_ptr<Query_base>(new NotQuery(std::move(operand)));
}

class BinaryQuery : public Query_base {
protected:
    BinaryQuery(Query l, Query r, char s) : lhs(std::move(l)), rhs(std::move(r)), opSym(s) {}

    std::string what() const override { return "( " + lhs.what() + ' ' + opSym + ' ' + rhs.what() + " )"; }

    Query lhs, rhs;
    char opSym;
};

class AndQuery : public BinaryQuery {
    friend Query operator&(Query lhs, Query rhs);

private:
    AndQuery(Query left, Query right) : BinaryQuery(std::move(left), std::move(right), '&') {}

    QueryResult eval(const TextQuery &t) const override;

};

inline QueryResult AndQuery::eval(const TextQuery &text) const {
    auto left = lhs.eval(text), right = rhs.eval(text);
    auto and_lines = std::make_shared<std::set<line_no>>();
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
                          std::inserter(*and_lines, and_lines->begin()));
    return QueryResult(what(), and_lines, left.get_file());
}

inline Query operator&(Query lhs, Query rhs) {
    return std::shared_ptr<Query_base>(new AndQuery(std::move(lhs), std::move(rhs)));
}

class OrQuery : public BinaryQuery {
    friend Query operator|(Query lhs, Query rhs);

private:
    OrQuery(Query left, Query right) : BinaryQuery(std::move(left), std::move(right), '|') {}

    QueryResult eval(const TextQuery &t) const override;

};

inline QueryResult OrQuery::eval(const TextQuery &text) const {
    auto left = lhs.eval(text), right = rhs.eval(text);
    auto or_lines = std::make_shared<std::set<line_no>>();
    std::set_union(left.begin(), left.end(), right.begin(), right.end(), std::inserter(*or_lines, or_lines->begin()));
    return QueryResult(what(), or_lines, left.get_file());
}

inline Query operator|(Query lhs, Query rhs) {
    return std::shared_ptr<Query_base>(new OrQuery(std::move(lhs), std::move(rhs)));
}

#endif //C_PRIMER_5TH_QUERY_H
