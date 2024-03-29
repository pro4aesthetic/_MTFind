#pragma once

#ifndef MATCH_PATTERN_
#define MATCH_PATTERN_

#include "stdafx2.h"

std::mutex mtx;

struct occurrences
{
    occurrences() noexcept
        : line("0"),
        pos_in_line(0),
        str("NOON") {
    }
    std::string line;
    int pos_in_line;
    std::string str;
};

namespace tools
{
    namespace detail_match_pattern
    {
        template<class ch>
        occurrences match_pattern(const ch* s, const ch* p) noexcept
        {
            assert(s);
            assert(p);

            std::unique_lock<std::mutex> ul(mtx);

            const ch* rp = p;
            occurrences match;
            std::string _s, _line;
            int _pos = 0, sp = -2;

            while (*s)
            {
                if (*s == ' ')
                    ++sp,
                    _s.clear();

                else if (isdigit(*s))
                    _line += *s;

                else
                {
                    if (*s == *p || *p == '?')
                    {
                        ++p,
                            _s += *s;
                        if (!*p)
                            match.line = _line,
                            match.pos_in_line = _pos / 2 + sp,
                            match.str = _s;
                    }
                    else if (s)
                        --s,
                        p = rp;
                    ++_pos;
                }
                ++s;
            }

            ul.unlock();

            return match;
        }

    }//namespace detail_match_pattern

    template<class s1, class s2>
    occurrences match_pattern(const s1& text, const s2& pattern) noexcept
    {
        return detail_match_pattern::match_pattern(
            &text[0], &pattern[0]
        );
    }

} // namespace tools


#endif // !MATCH_PATTERN_