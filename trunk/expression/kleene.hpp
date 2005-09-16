/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_KLEENE_HPP
#define   MACHINA_EXPRESSION_KLEENE_HPP

#include "base.hpp"
#include "weighted_expression.hpp"
#include "concat.hpp"

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
struct kleene_expression 
: unary_expression< Expr, kleene_expression<Expr> >
{
    typedef unary_expression< Expr, kleene_expression<Expr> >
            base_type;
    
    kleene_expression ( const Expr& expr ) : base_type(expr) {}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return apply_kleene(sm,base_type::expr(),out);
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr, typename Wt>
expression_arg<SM>& apply_kleene(SM& sm, 
                                 const weighted_expression<Wt,Expr>& expr,
                                 expression_arg<SM>& arg)
{
    expr.apply(sm,arg);
    typename state_machine_traits<SM>::state s = add_state(sm);
    typename expression_arg<SM>::connect_iterator citr, cend;
    typename expression_arg<SM>::to_iterator titr, tend;
    typename expression_arg<SM>::from_iterator fitr, fend;
    
    Wt repeat_wt = expr.weight();
    Wt out_wt    = Wt(1.0) - repeat_wt;
    
    citr = arg.connection_begin(); cend = arg.connection_end();
    for (;citr != cend; ++citr) {
        add_transition(sm, s, s, citr->tok, citr->wt*repeat_wt);
    }
    titr = arg.to_begin(); tend = arg.to_end();
    for (;titr != tend; ++titr) {
        add_transition(sm, s, titr->to, titr->tok, titr->wt*repeat_wt);
    }
    fitr = arg.from_begin(); fend = arg.from_end();
    for (;fitr != fend; ++fend) {
        add_transition(sm, fitr->from, s, fitr->tok, fitr->wt);
    }
    
    arg.clear();
    
    *(arg.from_inserter()) = 
    connection_from<SM>(s, state_machine_traits<SM>::epsilon(), out_wt);
    
    *(arg.to_inserter()) =
    connection_to<SM>(state_machine_traits<SM>::epsilon(), Wt(1.0), s);
    
    return arg;
}

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr>
expression_arg<SM>& apply_kleene(SM& sm, 
                                 const expression<Expr>& expr,
                                 expression_arg<SM>& arg)
{
    expr.apply(sm,arg);
    typename state_machine_traits<SM>::state s = add_state(sm);
    typename expression_arg<SM>::connect_iterator citr, cend;
    typename expression_arg<SM>::to_iterator titr, tend;
    typename expression_arg<SM>::from_iterator fitr, fend;
    
    typename state_machine_traits<SM>::weight_type wt(0.5);
    
    citr = arg.connection_begin(); cend = arg.connection_end();
    for (;citr != cend; ++citr) {
        add_transition(sm, s, s, citr->tok,citr->wt*wt);
    }
    titr = arg.to_begin(); tend = arg.to_end();
    for (;titr != tend; ++titr) {
        add_transition(sm, s, titr->to, titr->tok, titr->wt*wt);
    }
    fitr = arg.from_begin(); fend = arg.from_end();
    for (;fitr != fend; ++fend) {
        add_transition(sm, fitr->from, s, fitr->tok, fitr->wt);
    }
    
    arg.clear();
    
    *(arg.from_inserter()) = 
    connection_from<SM>(s, state_machine_traits<SM>::epsilon(), 0.5);
    
    *(arg.to_inserter()) =
    connection_to<SM>(state_machine_traits<SM>::epsilon(), 1.0, s);
    
    return arg;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
kleene_expression<Expr> operator* (const expression<Expr>& expr)
{
    return kleene_expression<Expr>(expr.derived());
}

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina

#endif // MACHINA_EXPRESSION_KLEENE_HPP
