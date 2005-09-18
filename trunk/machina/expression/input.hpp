/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_INPUT_HPP
#define   MACHINA_EXPRESSION_INPUT_HPP

#include "base.hpp"
#include <string>

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Input>
class input_expression : public expression< input_expression<Input> >
{
public:
    typedef Input input_type;
    
    input_expression(input_type in) : in(in){}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return apply_input(sm,out,in);
    }

private:
    input_type in;
};

////////////////////////////////////////////////////////////////////////////////

template <>
class input_expression<std::string> 
: public expression< input_expression<std::string> >
{
public:
    typedef std::string input_type;
    
    input_expression(input_type in) : in(in){}
    
    input_expression(const char* in) : in(in){}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        return apply_input(sm,out,in);
    }
    
private:
    input_type in;
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM, typename Input>
typename boost::enable_if< is_acceptor<SM>,expression_arg<SM>& >::type
apply_input (SM& sm, expression_arg<SM>& out, Input in)
{
    typename expression_arg<SM>::connect_insert_iterator 
             ci(out.connection_inserter());
    *ci = connection<SM>(in,(1.0)); 
    return out;
}

template <typename SM, typename Input>
typename boost::enable_if< is_transducer<SM>,expression_arg<SM>& >::type
apply_input (SM& sm, expression_arg<SM>& out, Input in)
{
    typename expression_arg<SM>::connect_insert_iterator 
             ci(out.connection_inserter());
             
    typename state_machine_traits<SM>::token_type
    p(in,state_machine_traits<SM>::output_epsilon());
    
    *ci = connection<SM>(p,(1.0)); 
    return out;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Input>
input_expression<Input> input_expr ( Input in )
{
    return input_expression<Input>(in);
}

inline input_expression<std::string> input_expr ( const char* in )
{
    return input_expression<std::string>(std::string(in));
}

////////////////////////////////////////////////////////////////////////////////

}; // namespcae machina

#endif // MACHINA_EXPRESSION_INPUT_HPP