/*============================================================================  Copyright 2005 Michael Pust.    Distributed under the Boost Software License, Version 1.0.  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying   LICENSE file.   ============================================================================*/

#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <iostream>

#include <machina/expression.hpp>
#include <machina/finite_sa.hpp>
#include <machina/finite_st.hpp>
#include <machina/graphviz.hpp>
#include <machina/logmath_io.hpp>
#include <machina/runtime_composition.hpp>

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_UNIT_TEST(test_composition)
{
    using namespace machina;
    using namespace std;
    
    typedef finite_st<int,string> ST;
    typedef finite_sa<string>     SA;

    runtime_expression<ST> e1 = +( inout_expr(1,"foo") >> inout_expr(2,"bar") |
                                   input_expr(3) ) >> inout_expr(2,"baz");
                                
    runtime_expression<SA> e2 = +( (input_expr("foo") >> "bar") | ("baz") );
    
    SA sm; set_equal(sm,e2);
    ST st; set_equal(st,e1);
    
    runtime_composition<ST,SA> compose_sm(st,sa);
}