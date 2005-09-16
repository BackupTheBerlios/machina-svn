/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#include <boost/test/auto_unit_test.hpp>
#include "../runtime_finite_sm.hpp"
#include "../graphviz.hpp"
#include <iostream>
#include "../logmath_io.hpp"

using namespace std;
using namespace machina;

template < typename SM >
typename state_machine_traits<SM>::state
connect ( SM& sm, 
          typename state_machine_traits<SM>::state s1, 
          typename state_machine_traits<SM>::state s2 )
{
    using namespace std;
    typedef typename state_machine_traits<SM>::transition_iterator_pair
                     trans_itr_pair;
    typedef typename state_machine_traits<SM>::state state;
    
    state new_state = add_state(sm, is_final(sm, s2));
    
    trans_itr_pair p = transitions(sm,s1);        
    for (; p.first != p.second; ++p.first) {
        assert (to(sm,*(p.first)) == from(sm,*(p.first)));
    }

    p = transitions(sm, s2);
    for (; p.first != p.second; ++p.first) {
        if(to(sm, *(p.first)) != from(sm,*(p.first))) {
            add_transition(sm, new_state, to(sm, *(p.first)), 
                           get_input(sm, *(p.first)), get_weight(sm, *(p.first)));
        } else {
            add_transition(sm, new_state, new_state, 
                           get_input(sm, *(p.first)), get_weight(sm, *(p.first)));   
        }
    }
    
    rev_trans_itr_pair q = reverse_transitions(sm, s1);
    for (; q.first != q.second; ++q.first) { 
        if(to(sm, *(q.first)) != from(sm, *(q.first))) {
            add_transition(sm, from(sm, *(q.first)), new_state, 
                           get_input(sm, *(q.first)), get_weight(sm, *(q.first)));
        } else {
            add_transition(sm, new_state, new_state, 
                           get_input(sm, *(q.first)), get_weight(sm, *(q.first)));   
        }
    }
    
    remove_state(sm, s1);
    remove_state(sm, s2);
    
    return new_state;
}   

BOOST_AUTO_UNIT_TEST(test_connect)
{
    using namespace machina;
    using namespace std;
    
    typedef runtime_finite_sm<> SM;
    
    SM sm;   
    state_machine_traits<SM>::state s1, s2, s3, s5, s6, s7;
    
    s1 = add_state(sm);
    s2 = add_state(sm);
    s3 = add_state(sm, true);
    
    s5 = add_state(sm);
    s6 = add_state(sm);
    s7 = add_state(sm, true);
    
    add_transition(sm,s1,s2,string("foo"),1.0);
    add_transition(sm,s2,s2,string("bar"),0.5);
    add_transition(sm,s2,s3,state_machine_traits<SM>::input_epsilon(), 0.5);
    
    add_transition(sm,s5,s6,string("bif"),1.0);
    add_transition(sm,s6,s6,string("foz"),0.3);
    add_transition(sm,s6,s7,state_machine_traits<SM>::input_epsilon(), 0.7);
    
    cout << "before:"     << endl;
    to_graphviz(cout, sm) << endl;
    
    s3 = connect(sm,s3,s5);  
    
    cout << "after:"      << endl;
    to_graphviz(cout, sm) << endl;
}
    