/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_ALTERNATIVE_HPP
#define   MACHINA_EXPRESSION_ALTERNATIVE_HPP

#include "base.hpp"
#include "weighted_expression.hpp"

//#include <iostream>

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Expr1, typename Expr2>
struct alternative_expression
: binary_expression< Expr1, Expr2, alternative_expression<Expr1,Expr2> >
{
    typedef binary_expression< Expr1, Expr2, 
                               alternative_expression<Expr1,Expr2> >
            base_type;
            
    alternative_expression(const Expr1& expr1, const Expr2& expr2)
    : base_type(expr1, expr2) {}
    
    template <typename SM>
    expression_arg<SM>& apply (SM& sm, expression_arg<SM>& out) const
    {
        return apply_append( sm
                           , base_type::left_expr()
                           , base_type::right_expr()
                           , out);
    } 
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr1, typename Expr2, typename Wt>
expression_arg<SM>& apply_append ( SM& sm,
                                   const weighted_expression<Wt,Expr1>& left,
                                   const weighted_expression<Wt,Expr2>& right, 
                                   expression_arg<SM>& out)
{   
    expression_arg<SM> arg;
        
    right.apply(sm,arg);
    left.apply (sm,out);
    
    typename state_machine_traits<SM>::weight_type wt_out = left.weight();
    typename state_machine_traits<SM>::weight_type wt_to  = right.weight();
              
    typename expression_arg<SM>::connect_iterator citr, cend;
    citr = out.connection_begin();
    cend = out.connection_end(); 
    for (;citr != cend; ++citr) citr->wt *= wt_out;
    
    citr = arg.connection_begin();
    cend = arg.connection_end();
    
    typename expression_arg<SM>::to_iterator titr, tend;
    titr = out.to_begin(); 
    tend = out.to_end();
    for (;titr != tend; ++titr) titr->wt *= wt_out;
    
    titr = arg.to_begin(); 
    tend = arg.to_end();
    
    typename expression_arg<SM>::from_iterator fitr, fend;
//    fitr = out.from_begin();
//    fend = out.from_end();
//    for (;fitr != fend; ++fitr) fitr->wt *= wt_out;
    
    fitr = arg.from_begin();
    fend = arg.from_end();
    
    typename expression_arg<SM>::to_insert_iterator 
             tin(out.to_inserter());
    typename expression_arg<SM>::from_insert_iterator 
             fin(out.from_inserter());
    typename expression_arg<SM>::connect_insert_iterator
             cin(out.connection_inserter());
             
    for (;citr != cend; ++citr) {
        citr->wt *= wt_to;
        *cin++    = *citr;
    }
    for (;titr != tend; ++titr) {
        titr->wt *= wt_to;
        *tin++    = *titr;
    }
    for (;fitr != fend; ++fitr) {
//        fitr->wt *= wt_to;
        *fin++    = *fitr;
    }
    
    return out;
} 


////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr1, typename Expr2, typename Wt>
expression_arg<SM>& apply_append ( SM& sm,
                                   const weighted_expression<Wt,Expr1>& left,
                                   const expression<Expr2>& right, 
                                   expression_arg<SM>& out)
{   
    expression_arg<SM> arg;
        
    right.derived().apply(sm, arg);
    left.apply(sm, out);
    
    typename state_machine_traits<SM>::weight_type wt_out = left.weight();
              
    typename expression_arg<SM>::connect_iterator citr, cend;
    citr = out.connection_begin();
    cend = out.connection_end(); 
    for (;citr != cend; ++citr) citr->wt *= wt_out;
    
    citr = arg.connection_begin();
    cend = arg.connection_end();
    
    typename expression_arg<SM>::to_iterator titr, tend;
    titr = out.to_begin(); 
    tend = out.to_end();
    for (;titr != tend; ++titr) titr->wt *= wt_out;
    
    titr = arg.to_begin(); 
    tend = arg.to_end();
    
    typename expression_arg<SM>::from_iterator fitr, fend;
//    fitr = out.from_begin();
//    fend = out.from_end();
//    for (;fitr != fend; ++fitr) fitr->wt *= wt_out;
    
    fitr = arg.from_begin();
    fend = arg.from_end();
    
    typename expression_arg<SM>::to_insert_iterator 
             tin(out.to_inserter());
    typename expression_arg<SM>::from_insert_iterator 
             fin(out.from_inserter());
    typename expression_arg<SM>::connect_insert_iterator
             cin(out.connection_inserter());
             
    for (;citr != cend; ++citr) { *cin++ = *citr; }
    for (;titr != tend; ++titr) { *tin++ = *titr; }
    for (;fitr != fend; ++fitr) { *fin++ = *fitr; }
    
    return out;
} 

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr1, typename Expr2, typename Wt>
expression_arg<SM>& apply_append ( SM& sm,
                                   const expression<Expr1>& left,
                                   const weighted_expression<Expr2,Wt>& right,
                                   expression_arg<SM>& out )
{      
    expression_arg<SM> arg;
        
    right.apply(sm, arg);
    left.derived().apply(sm, out);

    typename state_machine_traits<SM>::weight_type wt_to  = right.weight();  
    
    typename expression_arg<SM>::connect_iterator citr, cend;
    citr = arg.connection_begin();
    cend = arg.connection_end();
    
    typename expression_arg<SM>::to_iterator titr, tend;
    titr = arg.to_begin(); 
    tend = arg.to_end();
    
    typename expression_arg<SM>::from_iterator fitr, fend;
    fitr = arg.from_begin();
    fend = arg.from_end();
    
    typename expression_arg<SM>::to_insert_iterator 
             tin(out.to_inserter());
    typename expression_arg<SM>::from_insert_iterator 
             fin(out.from_inserter());
    typename expression_arg<SM>::connect_insert_iterator
             cin(out.connection_inserter());
             
    for (;citr != cend; ++citr) {
        citr->wt *= wt_to;
        *cin++    = *citr;
    }
    for (;titr != tend; ++titr) {
        titr->wt *= wt_to;
        *tin++    = *titr;
    }
    for (;fitr != fend; ++fitr) {
//        fitr->wt *= wt_to;
        *fin++    = *fitr;
    }
    
    return out;
}

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Expr1, typename Expr2>
expression_arg<SM>& apply_append ( SM& sm,
                                   const expression<Expr1>& left,
                                   const expression<Expr2>& right,
                                   expression_arg<SM>& out )
{
    expression_arg<SM> arg;
        
    right.derived().apply(sm, arg);
    left.derived().apply(sm, out);
    
    typename expression_arg<SM>::connect_iterator citr, cend;
    citr = arg.connection_begin();
    cend = arg.connection_end();
    
    typename expression_arg<SM>::to_iterator titr, tend;
    titr = arg.to_begin(); 
    tend = arg.to_end();
    
    typename expression_arg<SM>::from_iterator fitr, fend;
    fitr = arg.from_begin();
    fend = arg.from_end();
    
    typename expression_arg<SM>::to_insert_iterator 
             tin(out.to_inserter());
    typename expression_arg<SM>::from_insert_iterator 
             fin(out.from_inserter());
    typename expression_arg<SM>::connect_insert_iterator
             cin(out.connection_inserter());
             
    for (;citr != cend; ++citr) *cin++ = *citr;
    for (;titr != tend; ++titr) *tin++ = *titr;    
    for (;fitr != fend; ++fitr) *fin++ = *fitr;
    
    return out;
} 

////////////////////////////////////////////////////////////////////////////////

template <typename Expr1, typename Expr2>
alternative_expression<Expr1,Expr2>
operator | (const expression<Expr1>& expr1, const expression<Expr2>& expr2)
{
    return alternative_expression<Expr1,Expr2>( expr1.derived(),
                                                expr2.derived() ); 
}

////////////////////////////////////////////////////////////////////////////////

template <typename Expr>
alternative_expression< Expr,input_expression<std::string> >
operator | (const expression<Expr>& expr,const char* in)
{
    return 
    alternative_expression<Expr,input_expression<std::string> >
    (expr.derived(), input_expr(in));                                                      
}

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina

#endif // MACHINA_EXPRESSION_ALTERNATIVE_HPP