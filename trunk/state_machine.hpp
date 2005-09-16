/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef   MACHINA_STATE_MACHINE_HPP
#define   MACHINA_STATE_MACHINE_HPP

#include <boost/tuple/tuple_io.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/utility.hpp>
#include "tags.hpp"

namespace machina
{

template < typename SM, typename Enable = void >
struct state_machine_traits {};

////////////////////////////////////////////////////////////////////////////////
///
/// default traits for an state machine acceptor
///
////////////////////////////////////////////////////////////////////////////////
template < typename SM >
struct state_machine_traits< SM,
                             typename boost::enable_if<is_acceptor<SM> >::type >
{   
    typedef acceptor_tag               machine_class;
    typedef typename SM::state         state;
    typedef typename SM::transition    transition;
    typedef typename SM::token_type    token_type;
    typedef typename SM::token_type    input_type;
    typedef typename SM::weight_type   weight_type;
    
    typedef typename SM::transition_iterator transition_iterator;
    typedef typename SM::state_iterator      state_iterator; 
                     
    typedef std::pair<state_iterator,state_iterator>
            state_iterator_pair;
    
    typedef std::pair<transition_iterator,transition_iterator>
            transition_iterator_pair;
                     
    static  input_type epsilon() { return SM::epsilon(); }
    static  input_type input_epsilon() { return epsilon(); }
};

////////////////////////////////////////////////////////////////////////////////
///
/// default traits for an state machine transducer
///
////////////////////////////////////////////////////////////////////////////////
template < typename SM >
struct state_machine_traits<SM, 
                            typename boost::enable_if<is_transducer<SM> >::type>
{   
    typedef acceptor_tag               machine_class;
    typedef typename SM::state         state;
    typedef typename SM::transition    transition;
    typedef typename SM::input_type    input_type;
    typedef typename SM::output_type   output_type;
    typedef typename SM::weight_type   weight_type;
    
    typedef typename boost::tuple<input_type,output_type>    
                     token_type;
    
    typedef typename SM::transition_iterator transition_iterator;
    typedef typename SM::state_iterator      state_iterator; 
      
    typedef typename SM::inverse_transition_iterator
                     inverse_transition_iterator;
                     
    typedef std::pair<state_iterator,state_iterator>
            state_iterator_pair;
    
    typedef std::pair<transition_iterator,transition_iterator>
            transition_iterator_pair;
                     
    typedef std::pair<inverse_transition_iterator,
                      inverse_transition_iterator>
            inverse_transition_iterator_pair;
                     
    static  input_type  input_epsilon()  { return SM::input_epsilon(); }
    static  output_type output_epsilon() { return SM::output_epsilon(); }
    static  token_type epsilon()
            { return token_type(input_epsilon(),output_epsilon()); }
};

////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename state_machine_traits<SM>::state start_state (const SM& sm)
{
    return sm.start_state();
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
bool is_final (const SM& sm, typename state_machine_traits<SM>::state s)
{
    return sm.is_final(s);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename state_machine_traits<SM>::transition_iterator_pair 
transitions (const SM& sm, typename state_machine_traits<SM>::state s)
{
    return sm.transitions(s);
}
  
//////////////////////////////////////////////////////////////////////////////// 

template < typename SM >
typename state_machine_traits<SM>::transition_iterator_pair 
transitions (const SM& sm, 
             typename state_machine_traits<SM>::state s,
             typename state_machine_traits<SM>::input_type t)
{
    return sm.transitions(s,t);
}

////////////////////////////////////////////////////////////////////////////////   

template < typename SM >
typename state_machine_traits<SM>::inverse_transition_iterator_pair 
inverse_transitions (const SM& sm, typename state_machine_traits<SM>::state s)
{
    return sm.inverse_transitions(s);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename state_machine_traits<SM>::state_iterator_pair 
states (const SM& sm)
{
    return sm.states();
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM > 
typename state_machine_traits<SM>::input_type
get_input (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.get_input(t);
}
  
//////////////////////////////////////////////////////////////////////////////// 

template < typename SM >
typename state_machine_traits<SM>::output_type
get_output (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.get_output(t);
}
  
//////////////////////////////////////////////////////////////////////////////// 

template < typename SM >
typename state_machine_traits<SM>::token_type
get_token (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.get_token(t);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM > 
typename state_machine_traits<SM>::weight_type
get_weight (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.get_weight(t);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM > 
typename state_machine_traits<SM>::state
from (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.from(t);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM > 
typename state_machine_traits<SM>::state
to (const SM& sm, typename state_machine_traits<SM>::transition t)
{
    return sm.to(t);
}

////////////////////////////////////////////////////////////////////////////////
///
///  MutableStateMachine functions
///
////////////////////////////////////////////////////////////////////////////////

//\{

template < typename SM >
void 
set_final (SM& sm, typename state_machine_traits<SM>::state s, bool is_final)
{
    sm.set_final(s,is_final);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename state_machine_traits<SM>::state add_state (SM& sm,bool is_final=false)
{
    return sm.add_state(is_final);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
void remove_state (SM& sm, typename state_machine_traits<SM>::state s)
{
    sm.remove_state(s);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename boost::enable_if<is_acceptor<SM>,
                          typename state_machine_traits<SM>::transition>::type
add_transition (SM& sm,
                typename state_machine_traits<SM>::state       from,
                typename state_machine_traits<SM>::state       to,
                typename state_machine_traits<SM>::input_type  input,
                typename state_machine_traits<SM>::weight_type weight)
{
    return sm.add_transition(from,to,input,weight);
}

////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename boost::enable_if<is_transducer<SM>,
                          typename state_machine_traits<SM>::transition>::type
add_transition (SM& sm,
                typename state_machine_traits<SM>::state       from,
                typename state_machine_traits<SM>::state       to,
                typename state_machine_traits<SM>::input_type  input,
                typename state_machine_traits<SM>::weight_type weight)
{
    return sm.add_transition(from,
                             to,
                             input,
                             state_machine_traits<SM>::output_epsilon(),
                             weight);
}
  
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename boost::enable_if<is_transducer<SM>,
                          typename state_machine_traits<SM>::transition>::type
add_transition (SM& sm,
                typename state_machine_traits<SM>::state       from,
                typename state_machine_traits<SM>::state       to,
                typename state_machine_traits<SM>::input_type  input,
                typename state_machine_traits<SM>::output_type output,
                typename state_machine_traits<SM>::weight_type weight)
{
    return add_transition(from, to, input, output, weight);
}
             
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
typename boost::enable_if<is_transducer<SM>,
                          typename state_machine_traits<SM>::transition>::type
add_transition (SM& sm,
                typename state_machine_traits<SM>::state       from,
                typename state_machine_traits<SM>::state       to,
                typename state_machine_traits<SM>::token_type  tok,
                typename state_machine_traits<SM>::weight_type weight)
{
    return 
    sm.add_transition(from, to, boost::get<0>(tok), boost::get<1>(tok), weight);
}
             
////////////////////////////////////////////////////////////////////////////////  

template < typename SM >
void remove_transition (SM& sm, typename state_machine_traits<SM>::transition t)
{
    sm.remove_transition(t);
}
  
////////////////////////////////////////////////////////////////////////////////  

// \}



}; // namespace machina

#endif // MACHINA_STATE_MACHINE_HPP