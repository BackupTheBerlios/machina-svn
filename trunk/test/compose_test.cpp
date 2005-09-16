/*============================================================================  Copyright 2005 Michael Pust.    Distributed under the Boost Software License, Version 1.0.  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying   LICENSE file.   ============================================================================*/

#include <boost/test/auto_unit_test.hpp>
#include "../expression.hpp"
#include <string>
#include <iostream>
#include "../runtime_finite_sm.hpp"
#include "../runtime_finite_st.hpp"
#include "../graphviz.hpp"
#include "../logmath_io.hpp"
#include "../runtime_composition.hpp"

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_UNIT_TEST(test_composition)
{
    using namespace machina;
    using namespace std;
    
    typedef runtime_finite_st<int,string> ST;
    typedef runtime_finite_sm<string>     SM;

    runtime_expression<ST> e1 = +( inout_expr(1,"foo") >> inout_expr(2,"bar") |
                                   input_expr(3) ) >> inout_expr(2,"baz");
                                
    runtime_expression<SM> e2 = +( (input_expr("foo") >> "bar") | ("baz") );
    
    SM sm; set_equal(sm,e2);
    ST st; set_equal(st,e1);
    
    runtime_composition<SM,ST> compose_sm(sm,st);
}