/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_OPTIONAL_HPP
#define   MACHINA_EXPRESSION_OPTIONAL_HPP

#include "base.hpp"
#include "weighted_expression.hpp"

namespace machina {

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
struct optional_expression 
: unary_expression<Expr, optional_expression<Expr> >
{
    typedef unary_expression<Expr, optional_expression<Expr> >
            base_type;
            
    optional_expression(const Expr& expr)
    : base_type(expr) {}
    
    template <typename SM>
    expression_arg<SM>& apply (SM& sm, expression_arg<SM>& out) const
    {
        return apply_optional(sm, base_type::expr(), out);
    }
    
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr, typename Wt>
expression_arg<SM>&
apply_optional (SM& sm, 
                const weighted_expression<Wt, Expr>& expr, 
                expression_arg<SM>& arg) {
    expr.apply(sm,arg);
    
    Wt option_wt = expr.weight();
    Wt skip_wt   = 1.0 - option_wt;
    
    typename expression_arg<SM>::to_iterator      titr = arg.to_begin();
    typename expression_arg<SM>::to_iterator      tend = arg.to_end();
    typename expression_arg<SM>::connect_iterator citr = arg.connection_begin();
    typename expression_arg<SM>::connect_iterator cend = arg.connection_end();
    
    for (; titr != tend; ++titr) { titr->wt *= option_wt; }
    for (; citr != cend; ++citr) { citr->wt *= option_wt; }
    
    *(arg.connection_inserter()) = 
      connection<SM>(state_machine_traits<SM>::epsilon(),skip_wt);
}

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr>
expression_arg<SM>&
apply_optional (SM& sm, const expression<Expr>& expr, expression_arg<SM>& arg) {
    expr.apply(sm,arg);
    
    typename expression_arg<SM>::to_iterator      titr = arg.to_begin();
    typename expression_arg<SM>::to_iterator      tend = arg.to_end();
    typename expression_arg<SM>::connect_iterator citr = arg.connection_begin();
    typename expression_arg<SM>::connect_iterator cend = arg.connection_end();
    
    for (; titr != tend; ++titr) { titr->wt *= 0.5; }
    for (; citr != cend; ++citr) { citr->wt *= 0.5; }
    
    *(arg.connection_inserter()) = 
      connection<SM>(state_machine_traits<SM>::epsilon(),0.5);
      
    return arg;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
optional_expression<Expr> 
operator ! (const expression<Expr>& expr)
{
    return optional_expression<Expr>(expr.derived());
}

}; //namespace machina

////////////////////////////////////////////////////////////////////////////////

#endif // MACHINA_EXPRESSION_OPTIONAL_HPP