/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_EPSILON_HPP
#define   MACHINA_EXPRESSION_EPSILON_HPP

#include "base.hpp"

namespace machina 
{

struct epsilon_expression : expression<epsilon_expression>
{
    template <typename SM>
    expression_arg<SM>& apply( SM& sm, expression_arg<SM>& out ) const;
};

inline epsilon_expression epsilon_expr()
{
    return epsilon_expression();
}

}; // namespace machina

#endif // MACHINA_EXPRESSION_EPSILON_HPP