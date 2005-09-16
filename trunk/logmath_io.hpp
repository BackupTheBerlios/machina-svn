/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_LOGMATH_IO_HPP
#define   MACHINA_LOGMATH_IO_HPP

#include "logprob.hpp"
#include "lognumber.hpp"
#include <iostream>

namespace machina { namespace logmath {

enum format { fmt_log = 1, fmt_exp = 2 };

class set_format {
public:
    set_format(format fmt) : fmt(fmt) {}
    std::ios_base& operator()(std::ios_base& io) const {
        io.iword(io_fmt_idx()) = fmt;
        return io;
    }
    
    static format current_format(std::ios_base& io) {
        long f = io.iword(io_fmt_idx());
        if (f == 0) { 
            io.iword(io_fmt_idx()) = static_cast<long>(fmt_log);
            return fmt_log;
        }
        return static_cast<format>(f);
    }
    
    static long io_fmt_idx() {
        static const long _io_fmt_idx = std::ios_base::xalloc();
        return _io_fmt_idx;  
    }
private:
    format fmt;
};

inline
std::ios_base& operator << (std::ios_base& io, set_format f) {
    return f(io);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision> inline 
std::ostream& operator<<(std::ostream& out, basic_logprob<Precision> p) {
    if (set_format::current_format(out) == fmt_log) { out << p.m_log; }
    else { out << basic_logprob<Precision>::c_util.exp(p.m_log); }
    return out;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>            
std::ostream& 
operator << (std::ostream& out, basic_lognumber<Precision> p) {
    if (set_format::current_format(out) == fmt_log) {return out << p.m_number;}
    else return out << basic_lognumber<Precision>::c_util.exp(p.m_number);
}

////////////////////////////////////////////////////////////////////////////////

} }; // namespace machina::logmath

#endif // MACHINA_LOGMATH_IO_HPP