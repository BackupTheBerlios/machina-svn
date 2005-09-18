/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_GRAPHVIZ_HPP
#define   MACHINA_GRAPHVIZ_HPP

#include <iostream>

namespace machina {
namespace detail {

template < typename SM >
std::ostream& print_final_states(std::ostream& os, const SM& sm)
{
    using namespace std;
    using namespace machina;
    typename state_machine_traits<SM>::state_iterator_pair p;
    p = states(sm);
    for (; p.first != p.second; ++p.first) if (is_final(sm, *(p.first))) {
        os << "s_" << *(p.first) << " ";
    } 
    return os;
}

template < typename SM >
std::ostream& print_transitions (std::ostream& os, const SM& sm)
{
    using namespace std;
    using namespace machina;
    
    typedef typename state_machine_traits<SM>::state_iterator           
                     state_itr;
    typedef typename state_machine_traits<SM>::transition_iterator      
                     trans_itr;
    typedef typename state_machine_traits<SM>::state_iterator_pair     
                     state_itr_pair;
    typedef typename state_machine_traits<SM>::transition_iterator_pair 
                     trans_itr_pair;
    
    state_itr_pair p = states(sm);
    for (;p.first != p.second; ++p.first) {
        trans_itr_pair tp = transitions(sm, *p.first);
        for (; tp.first != tp.second; ++tp.first) {
            os <<"\t"<<"s_"<< *p.first << " -> " << "s_" << to(sm, *tp.first)
               <<"[ label = \"" << get_token(sm, *tp.first) 
                                << " (" << get_weight(sm, *tp.first) << ")\" ];" 
                                << endl;
        }
    }
    return os;
}

}; //namespace machina::detail

template < typename SM >
std::ostream& to_graphviz(std::ostream& os, const SM& sm)
{
    using namespace std;
    
    os << "digraph fsm {"       << endl;
    os << "\t" << "rankdir=LR;" << endl; 
    os << "\t" << "node [label = \"\"];" << endl;
    os << "\t" << "node [shape = doublecircle];";
    detail::print_final_states(os,sm) << ";" << endl;
    os << "\t" << "node [shape = doublecircle]; s_"<< start_state(sm) <<" ;" <<  endl;
    os << "\t" << "node [shape = circle];" << endl;
    detail::print_transitions(os, sm) << endl;
    os << "}";
    
    return os;
} 

}; // namespace machina

#endif // MACHINA_GRAPHVIZ_HPP