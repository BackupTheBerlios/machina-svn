/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_WEIGHTED_EXPRESSION_HPP
#define   MACHINA_EXPRESSION_WEIGHTED_EXPRESSION_HPP

#include "base.hpp"

namespace machina 
{

////////////////////////////////////////////////////////////////////////////////

template <typename Wt, typename Expr>
struct weighted_expression 
: unary_expression< Expr, weighted_expression<Wt, Expr> >
{
    typedef unary_expression< Expr, weighted_expression<Wt, Expr> >
            base_type;
    
    weighted_expression ( Wt wt, const Expr& expr ) 
    : base_type(expr),wt(wt) {}
    
    /// ignores the weight when applied
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return base_type::expr().apply(sm,out);
    }
    
    Wt weight() const {return wt;}
private:
    Wt wt;
};

////////////////////////////////////////////////////////////////////////////////

template <typename Wt, typename Expr>
weighted_expression<Wt,Expr>
operator * (Wt wt, const expression<Expr>& expr)
{
    return weighted_expression<Wt,Expr>(wt,expr.derived());
}

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina

#endif // MACHINA_EXPRESSION_WEIGHTED_EXPRESSION_HPP