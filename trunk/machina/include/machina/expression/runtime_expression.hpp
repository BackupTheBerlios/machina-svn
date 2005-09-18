/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_RUNTIME_EXPRESSION_HPP
#define   MACHINA_EXPRESSION_RUNTIME_EXPRESSION_HPP

#include "base.hpp"
#include <list>

#include <boost/scoped_ptr.hpp>

namespace machina 
{

////////////////////////////////////////////////////////////////////////////////

template <typename SM>
class abstract_expression_container
{
public:
    virtual ~abstract_expression_container() {}
    
    virtual 
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const = 0;
    
    virtual abstract_expression_container* clone() const = 0;
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr>
class concrete_expression_container : 
public abstract_expression_container<SM>
{
public:
    typedef Expr expression_type;
    
    concrete_expression_container(const expression<Expr>& expr) 
    : expr(expr.derived()){}
    
    virtual abstract_expression_container<SM>* clone() const
    {
        return new concrete_expression_container(expr);
    }
    
    virtual ~concrete_expression_container() {}
    
    virtual 
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return expr.apply(sm,out);
    }
    
private:
    expression_type expr;
};

////////////////////////////////////////////////////////////////////////////////
template <typename SM>
class runtime_expression : public expression< runtime_expression<SM> >
{
public:
    template <typename Expr>
    runtime_expression& operator= (const expression<Expr>& expr)
    {
        ptr.reset(new concrete_expression_container<SM,Expr>(expr));
        return *this;
    }
    
    template<typename Expr>
    runtime_expression(const expression<Expr>& expr) 
    : ptr(new concrete_expression_container<SM,Expr>(expr)) {}
    
    runtime_expression(const runtime_expression& rt)
    : ptr(rt.ptr ? rt.ptr->clone() : 0) {}
    
    runtime_expression& operator= (const runtime_expression& rt_expr)
    {
        if (this != &rt_expr)
        {
            ptr.reset (rt_expr.ptr ? rt_expr.ptr->clone() : 0);
        }
        return *this; 
    }
    
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return ptr->apply(sm, out);
    }
    
private:
    boost::scoped_ptr< abstract_expression_container<SM> > ptr;
};


}; // namespace machina

////////////////////////////////////////////////////////////////////////////////

#endif // MACHINA_EXPRESSION_RUNTIME_EXPRESSION_HPP