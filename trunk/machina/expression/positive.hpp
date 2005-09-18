/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_POSITIVE_HPP
#define   MACHINA_EXPRESSION_POSITIVE_HPP

#include "base.hpp"
#include "weighted_expression.hpp"

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
struct positive_expression 
: unary_expression< Expr, positive_expression<Expr> >
{
    typedef unary_expression< Expr, positive_expression<Expr> >
            base_type;
    
    positive_expression ( const Expr& expr ) : base_type(expr) {}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return apply_positive(sm,base_type::expr(),out);
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr, typename Wt>
expression_arg<SM>& apply_positive(SM& sm, 
                                   const weighted_expression<Wt,Expr>& expr,
                                   expression_arg<SM>& out)
{
    return apply_positive_wt(sm,expr,out,expr.weight());
}

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr>
expression_arg<SM>& apply_positive(SM& sm, 
                                   const expression<Expr>& expr,
                                   expression_arg<SM>& out)
{
    return apply_positive_wt(sm,expr,out,0.5);
}

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr, typename Wt>
expression_arg<SM>& apply_positive_wt(SM& sm, 
                                      const expression<Expr>& expr,
                                      expression_arg<SM>& arg,
                                      Wt wt)
{
    expr.apply(sm,arg);
    
    expression_arg<SM> repeat_arg;
    expr.apply(sm,repeat_arg);
    
    typename state_machine_traits<SM>::state s = add_state(sm);
    
    typename expression_arg<SM>::connect_iterator citr, cend;
    typename expression_arg<SM>::to_iterator titr, tend;
    typename expression_arg<SM>::from_iterator fitr, fend;
    typename expression_arg<SM>::to_insert_iterator tin(arg.to_inserter());
    
    Wt repeat_wt = wt;
    Wt out_wt    = Wt(1.0) - repeat_wt;
    
    citr = arg.connection_begin(); cend = arg.connection_end();
    for (;citr != cend; ++citr) {
        *tin++ = connection_to<SM>(citr->tok, citr->wt, s);
    }
    
    fitr = arg.from_begin(); fend = arg.from_end();
    for (;fitr != fend; ++fitr) {
        add_transition(sm, fitr->from, s, fitr->tok, fitr->wt);
    }
    
    citr = repeat_arg.connection_begin(); cend = repeat_arg.connection_end();
    for (;citr != cend; ++citr) {
        add_transition(sm, s, s, citr->tok, citr->wt*repeat_wt);
    }
    
    titr = repeat_arg.to_begin(); tend = repeat_arg.to_end();
    for (;titr != tend; ++titr) {
        add_transition(sm, s, titr->to, titr->tok, titr->wt*repeat_wt);
    }
    
    fitr = repeat_arg.from_begin(); fend = repeat_arg.from_end();
    for (;fitr != fend; ++fend) {
        add_transition(sm, fitr->from, s, fitr->tok, fitr->wt);
    }
    
    arg.clear_from();
    arg.clear_connection();
    
    *(arg.from_inserter()) = 
    connection_from<SM>(s, state_machine_traits<SM>::epsilon(), out_wt);
    
    return arg;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
positive_expression<Expr> operator+ (const expression<Expr>& expr)
{
    return positive_expression<Expr>(expr.derived());
}

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina

#endif // MACHINA_EXPRESSION_KLEENE_HPP
