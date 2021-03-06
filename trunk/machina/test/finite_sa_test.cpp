/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#include <boost/test/auto_unit_test.hpp>

#include <machina/finite_sa.hpp>
#include <machina/logmath_io.hpp>

BOOST_AUTO_UNIT_TEST(test_machina_finite_sa)
{
    using namespace machina;
    using namespace std;
    
    typedef finite_sa<> SM;
    
    SM sm;
    
    // very cumbersome way of writing
    // sm = "foo" >> "bar";
    typedef state_machine_traits<SM>::state state;
    typedef state_machine_traits<SM>::transition_iterator trans_itr;
    typedef state_machine_traits<SM>::transition_iterator_pair trans_itr_pair;
    state s0 = start_state(sm);
    state s1 = add_state(sm);
    state s2 = add_state(sm,true);
    add_transition(sm,s0,s1,string("foo"),1.0);
    add_transition(sm,s1,s2,string("bar"),1.0);
    
    trans_itr_pair p = transitions(sm,s0);
    trans_itr trans_to_s1 = p.first;
    BOOST_CHECK_EQUAL( string("foo"),     get_input(sm, *trans_to_s1) );
    BOOST_CHECK_EQUAL( s1   ,     to(sm, *trans_to_s1) );
    BOOST_CHECK( ++p.first == p.second );
    BOOST_CHECK_EQUAL( false,     is_final(sm, s1) );
    
    p = transitions(sm,s0,string("foo"));
    BOOST_CHECK_EQUAL(true, p.first != p.second);
    
    p = transitions(sm,s1,string("foo"));
    BOOST_CHECK_EQUAL(true, p.first == p.second);
    
    p = transitions(sm,s0,string("bar"));
    BOOST_CHECK_EQUAL(true, p.first == p.second);

    p = transitions(sm,s1,string("bar"));
    BOOST_CHECK_EQUAL(true, p.first != p.second);    
    
    p = transitions(sm, s1);
    trans_itr trans_to_s2 = p.first;
    BOOST_CHECK_EQUAL( string("bar"),     get_input(sm, *trans_to_s2) );
    BOOST_CHECK_EQUAL( s2   ,     to(sm, *trans_to_s2 ) );
    BOOST_CHECK( ++p.first == p.second );
    BOOST_CHECK_EQUAL( true ,     is_final(sm, s2) );
    
    p = transitions(sm, s2);
    
    BOOST_CHECK( p.first == p.second );
}