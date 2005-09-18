/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_TRANSITION_SYMBOL_H
#define   MACHINA_TRANSITION_SYMBOL_H

#include  <iostream>
#include  <boost/functional/hash/hash.hpp>
#include  <boost/operators.hpp>

namespace machina {

class epsilon_cast : public std::exception {
public:
    virtual const char* what() throw () {
        return "epsilon cannot be cast to underlying type";
    }
    virtual ~epsilon_cast()    throw () {}
}; 

////////////////////////////////////////////////////////////////////////////////
///
///  \class token augments a tokenizable object with an epsilon symbol.
///
////////////////////////////////////////////////////////////////////////////////
template <typename T>
class token 
: public boost::totally_ordered<token<T>,T>
, public boost::totally_ordered<T> {
public:
    token(const T& m_tok):m_tok(m_tok),m_is_epsilon(false) { }
    token():m_is_epsilon(true) { }
    token& operator = (const T& t) { 
        m_tok = t; 
        m_is_epsilon = false; 
        return *this; 
    }
    token& operator = (const token& t) { 
        m_tok = t.m_tok; 
        m_is_epsilon = t.m_is_epsilon; 
        return *this; 
    }
    operator T const& () const { 
        if (m_is_epsilon) 
            throw epsilon_cast(); 
        return m_tok; 
    }
    bool is_epsilon() const {return m_is_epsilon;}
    static token epsilon()  {return token();}   
    
    bool operator == (T const& t) const {
        return !is_epsilon() and m_tok == t;
    }  
    bool operator == (token const& other) const {
        return is_epsilon() == other.is_epsilon() 
           and        m_tok == other.m_tok;
    }   
    bool operator < (token const& other) const {
        return !is_epsilon() and !other.is_epsilon() 
               ? m_tok < other.m_tok
               : is_epsilon() and !other.is_epsilon();            
    }  
    bool operator <  (T const& t) const {
        return is_epsilon() or m_tok < t;
    }    
    bool operator >  (T const& t) const {
        return !is_epsilon() and m_tok > t;
    }
private:
    T m_tok;
    bool m_is_epsilon;
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
std::size_t hash_value(token<T> const& t) {
    boost::hash<T> h;
    if (t.is_epsilon()) return 0;
    return h((T const&)(t));
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream& operator << (std::ostream& os, const token<T>& tok)
{
    if (tok.is_epsilon()) return os << "\\epsilon\\";
    else return os << (T const&)(tok);
}

} // namespace machina

#endif // MACHINA_TRANSITION_SYMBOL_H