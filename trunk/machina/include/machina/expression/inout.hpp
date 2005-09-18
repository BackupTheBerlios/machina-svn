/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_INOUT_HPP
#define   MACHINA_EXPRESSION_INOUT_HPP

#include "base.hpp"
#include <string>

namespace machina
{

////////////////////////////////////////////////////////////////////////////////

template <typename Input, typename Output>
class inout_expression : public expression< inout_expression<Input,Output> >
{
public:
    typedef Input  input_type;
    typedef Output output_type;
    
    inout_expression(input_type in, output_type out) : _in(in), _out(out) {}
    
    template <typename SM>
    expression_arg<SM>& apply ( SM& sm, expression_arg<SM>& out ) const
    {
        typename expression_arg<SM>::connect_insert_iterator 
                 ci(out.connection_inserter());
                 
        *ci = 
            connection<SM>(typename state_machine_traits<SM>::token_type(
                               typename state_machine_traits<SM>::input_type(_in),
                               typename state_machine_traits<SM>::output_type(_out)
                          ),1.0); 
        return out;
    }

private:
    input_type  _in;
    output_type _out;
};

////////////////////////////////////////////////////////////////////////////////

template <typename Input, typename Output>
inout_expression<Input, Output> inout_expr (Input in, Output out)
{
    return inout_expression<Input,Output>(in, out);
}

template <typename Input>
inout_expression<Input, std::string> inout_expr (Input in, const char* out)
{
    return inout_expression<Input,std::string>(in, std::string(out));
}

template <typename Output>
inout_expression<std::string, Output> inout_expr (const char* in, Output out)
{
    return inout_expression<std::string,Output>(std::string(in), out);
}

inline inout_expression<std::string, std::string> 
inout_expr (const char* in, const char* out)
{
    return inout_expression<std::string,std::string>(std::string(in), 
                                                     std::string(out));
}

////////////////////////////////////////////////////////////////////////////////

}; // namespcae machina

#endif // MACHINA_EXPRESSION_INOUT_HPP