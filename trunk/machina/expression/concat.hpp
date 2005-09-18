/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_CONCAT_HPP
#define   MACHINA_EXPRESSION_CONCAT_HPP

#include "base.hpp"

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Expr1, typename Expr2>
struct concat_expression 
: binary_expression< Expr1, Expr2, concat_expression<Expr1, Expr2> >
{
    typedef binary_expression< Expr1, Expr2, concat_expression<Expr1, Expr2> >
            base_type;
    
    concat_expression ( const Expr1& expr1, const Expr2& expr2 ) 
    : base_type(expr1, expr2) {}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        expression_arg<SM> right_arg;
        expression_arg<SM> left_arg;
        
        base_type::right_expr().apply(sm, right_arg);
        base_type::left_expr().apply(sm, left_arg);
        
        /// \todo replace with algorithm implementation
        typename expression_arg<SM>::connect_iterator citr, cend;
        typename expression_arg<SM>::to_iterator titr, tend;
        typename expression_arg<SM>::from_iterator fitr, fend;
        typename expression_arg<SM>::to_insert_iterator tin(out.to_inserter());
        typename expression_arg<SM>::from_insert_iterator fin(out.from_inserter());
        
        typename state_machine_traits<SM>::state s = add_state(sm);
        
        /// x -in,wt->  becomes x -in,wt-> s  
        fitr = left_arg.from_begin(); fend = left_arg.from_end();
        for (; fitr != fend; ++fitr) {
            add_transition(sm, fitr->from, s, fitr->tok, fitr->wt);
        }
        
        /// -in,wt-> x becomes s -in,wt-> x
        titr = right_arg.to_begin(); 
        tend = right_arg.to_end();
        for (; titr != tend; ++titr) {
            add_transition(sm, s, titr->to, titr->tok, titr->wt);
        }
            
        /// -in,wt->  becomes -in,wt->s
        citr = left_arg.connection_begin(); 
        cend = left_arg.connection_end();
        for (; citr != cend; ++citr) { 
                *tin++ = connection_to<SM>(citr->tok, citr->wt, s);
        }
        
        citr = right_arg.connection_begin();
        cend = right_arg.connection_end();
        for (; citr != cend; ++citr) {
            *fin++ = connection_from<SM>(s, citr->tok, citr->wt);
        }
        
        titr = left_arg.to_begin();
        tend = left_arg.to_end();
        for (; titr != tend; ++titr) {
            *tin++ = *titr;
        }
        
        fitr = right_arg.from_begin();
        fend = right_arg.from_end();
        for (; fitr != fend; ++fitr) {
            *fin++ = *fitr;
        }
        
        return out;
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename Expr1, typename Expr2>
concat_expression<Expr1,Expr2>
operator >> (const expression<Expr1>& expr1, const expression<Expr2>& expr2)
{
    return concat_expression<Expr1,Expr2>(expr1.derived(), expr2.derived());
} 

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
concat_expression<Expr,input_expression<std::string> >
operator >> (const expression<Expr>& expr, const char* in)
{
    return concat_expression<Expr,input_expression<std::string> >
           (expr.derived(),input_expression<std::string>(in));
}

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
concat_expression<input_expression<std::string>, Expr >
operator >> (const char* in, const expression<Expr>& expr)
{
    return concat_expression<input_expression<std::string>,Expr >
           (input_expression<std::string>(in),expr.derived());
}

////////////////////////////////////////////////////////////////////////////////
}; //namespace machina

#endif // MACHINA_EXPRESSION_CONCAT_HPP