/*============================================================================  Copyright 2005 Michael Pust.    Distributed under the Boost Software License, Version 1.0.  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying   LICENSE file.   ============================================================================*/

#include <boost/test/auto_unit_test.hpp>

#include <string>
#include <iostream>

#include <machina/expression.hpp>
#include <machina/finite_sa.hpp>
#include <machina/finite_st.hpp>
#include <machina/graphviz.hpp>
#include <machina/logmath_io.hpp>

BOOST_AUTO_UNIT_TEST(test_machina_sm_expression)
{
    using namespace machina;
    using namespace std;

    BOOST_CHECK_EQUAL( typeid(input_expr("hello") >> "world").name() ,
                       typeid(concat_expression< input_expression<string>,
                                                 input_expression<string> >).name() );
 

    BOOST_CHECK_EQUAL( typeid( *( 0.5 * (input_expr("hi") >> "there") ) ).name() ,
                 typeid(kleene_expression<
                          weighted_expression<double,
                                              concat_expression
                                                 <
                                                 input_expression<string>,
                                                 input_expression<string>
                                                 >
                                             >
                        > ).name() );  
         
    BOOST_CHECK_EQUAL( 
        typeid( input_expr("hi") | "hello" | "hey" ).name() ,
        typeid( alternative_expression<
                    alternative_expression< 
                        input_expression<string>,
                        input_expression<string> 
                    >,
                    input_expression<string>
                > ).name()
    );
    
    runtime_expression<finite_sa<> > rt = 
    +((0.5*input_expr("hello") | 0.5*input_expr("hi")) >> 
      "there" >> "world") >> ("michael" >> !input_expr("pust"));
    
    finite_sa<> sm;
    set_equal(sm,rt);
    
    cout << logmath::set_format(logmath::fmt_exp);
    to_graphviz(cout,sm) << endl;
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_UNIT_TEST(test_machina_st_expression)
{
    using namespace machina;
    using namespace std;

    BOOST_CHECK_EQUAL( typeid(input_expr("hello") >> "world").name() ,
                       typeid(concat_expression< 
                                     input_expression<string>,
                                     input_expression<string> >).name() );
 

    BOOST_CHECK_EQUAL(typeid(*(0.5 * (input_expr("hi") >> "there"))).name() ,
                 typeid(kleene_expression<
                          weighted_expression<double,
                                              concat_expression
                                                 <
                                                 input_expression<string>,
                                                 input_expression<string>
                                                 >
                                             >
                        > ).name() );  
         
    BOOST_CHECK_EQUAL( 
        typeid( input_expr("hi") | "hello" | "hey" ).name() ,
        typeid( alternative_expression<
                    alternative_expression< 
                        input_expression<string>,
                        input_expression<string> 
                    >,
                    input_expression<string>
                > ).name()
    );
    
    runtime_expression<finite_st<> > rt = 
    +((0.5*input_expr("hello") | 0.5*input_expr("hi")) >> 
      "there" >> "world") >> ("michael" >> !input_expr("pust"));
    
    finite_st<> st;
    set_equal(st,rt);
    
    cout << logmath::set_format(logmath::fmt_exp);
    to_graphviz(cout,st) << endl;
}