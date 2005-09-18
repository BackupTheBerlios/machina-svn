/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_BASE
#define   MACHINA_EXPRESSION_BASE

#include "../state_machine.hpp"
#include "arg.hpp"

namespace machina 
{


////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
class expression
{
public:
    typedef Expr derived_type;
    
    const derived_type& derived() const
    {
        return *static_cast<derived_type const*>(this);
    }
    
    derived_type& derived()
    {
        return *static_cast<derived_type*>(this);
    }
    
    template <typename SM>
    expression_arg<SM>& 
    apply (SM& sm, expression_arg<SM>& out) const
    {
        return derived().apply(sm, out);
    }
};

////////////////////////////////////////////////////////////////////////////////

template <typename Expr, typename Return>
class unary_expression : public expression<Return>
{
public:
    typedef expression<Return> return_type;
    typedef Expr               expression_type;
    
    unary_expression (const expression_type& _expr) :
    _expr(_expr) {}
    
    const expression_type& expr() const {return _expr;}
    
private:
    expression_type _expr;    
};

////////////////////////////////////////////////////////////////////////////////

template <typename Expr1, typename Expr2, typename Return>
class binary_expression : public expression<Return>
{
public:
    typedef expression<Return>  return_type;
    typedef Expr1               left_expression_type;
    typedef Expr2               right_expression_type;
    
    binary_expression (const left_expression_type&  _left, 
                       const right_expression_type& _right) 
    : _left(_left) , _right(_right) {}
    
    const left_expression_type&  left_expr () const {return _left;}
    const right_expression_type& right_expr() const {return _right;}
    
private:
    left_expression_type  _left; 
    right_expression_type _right;   
};

}; // namespace machina

#endif // MACHINA_EXPRESSION_BASE