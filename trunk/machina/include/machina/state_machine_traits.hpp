/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef   MACHINA_STATE_MACHINE_TRAITS_HPP
#define   MACHINA_STATE_MACHINE_TRAITS_HPP

#include  <boost/utility.hpp>
#include  <machina/tags.hpp>

namespace machina {


template <class T, class S = void>
struct concept_traits_specialized {
    enum { value = false };
};

template <class T> 
struct concept_traits_specialized<
           T
         , typename enable_if_defined<
                      typename T::concept_traits_specialization
                    >::type
       > {
    enum { value = true };
};

template <class T, class Specialize = void> 
struct concept_traits {};

template <class T>
struct concept_traits<
           T
         , typename boost::disable_if<concept_traits_specialized<T> >::type
       > {
    typedef typename T::machine_class machine_class;
    enum m { modifiable = T::modifiable };
    enum t { transition_modifiable = T::transition_modifiable };
    enum i { invertible = T::invertible };
    enum f { finite_state = T::finite_state };
};

template <typename SM> struct is_acceptor {
    enum { value = is_acceptor_tag<
                     typename concept_traits<SM>::machine_class
                   >::value };
};

template <typename SM> struct is_transducer {
    enum { value = is_transducer_tag<
                     typename concept_traits<SM>::machine_class
                   >::value };
};

template <typename SM> struct is_invertible {
    enum { value = concept_traits<SM>::invertible };
};

template <typename SM> struct is_modifiable {
    enum { value = concept_traits<SM>::modifiable };
};

template <typename SM> 
struct is_modifiable_transducer {
    enum { value = concept_traits<SM>::modifiable &&
                   is_transducer_tag<
                     typename concept_traits<SM>::machine_class
                   >::value };
};

template <typename SM> 
struct is_finite_state {
    enum { value =concept_traits<SM>::finite_state };
};

///////////////////////////////////////////////////////////////////////////////

template <class SM, class Specialized = void>
struct modifiable_specialized {
    enum {value=false};  
};

template <class SM>
struct modifiable_specialized<
           SM
         , typename enable_if_defined<typename SM::modifiable_specialization>::type
       > {
    enum { value = true };           
};

template <class SM, class Specialized = void>
struct transition_modifiable_specialized {
    enum {value=false};  
};

template <class SM>
struct transition_modifiable_specialized<
           SM
         , typename enable_if_defined<typename SM::transition_modifiable_specialization>::type
       > {
    enum { value = true };           
};

///////////////////////////////////////////////////////////////////////////////

template <class T, class MC = void, class I = void, class Extend = void>
struct invertible_traits {};

template <class T, class S = void>
struct invertible_specialized {
    enum { value = false };
};

template <class T> 
struct invertible_specialized<
           T
         , typename enable_if_defined<typename T::invertible_specialization>::type
       > {
    enum { value = true };
};

template <class T>
struct invertible_traits< 
           T
         , typename boost::enable_if<is_transducer<T> >::type
         , typename boost::enable_if<is_invertible<T> >::type
         , typename boost::disable_if<invertible_specialized<T> >::type
       > {
    typedef typename T::inverse_transition_iterator 
            inverse_transition_iterator;
    typedef std::pair<inverse_transition_iterator,inverse_transition_iterator>
            inverse_transition_iterator_pair;
}; 

///////////////////////////////////////////////////////////////////////////////

template <class T, class FS = void, class Extend = void> 
struct finite_state_traits {};

template <class T, class S = void>
struct finite_state_specialized {
    enum { value = false };
};

template <class T> 
struct finite_state_specialized<
           T
         , typename enable_if_defined<typename T::finite_state_specialization>::type
       > {
    enum { value = true };
};

template <class T>
struct finite_state_traits< 
           T
         , typename boost::enable_if<is_finite_state<T> >::type
         , typename boost::disable_if<finite_state_specialized<T> >::type
       > {
    typedef typename T::state_iterator state_iterator;  
    typedef std::pair<state_iterator,state_iterator>
            state_iterator_pair;                            
};

///////////////////////////////////////////////////////////////////////////////

template <class SM, class Specialize = void>
struct core_traits {};

template <class T, class S = void>
struct core_specialized {
    enum { value = false };
};

template <class T> 
struct core_specialized<
           T
         , typename enable_if_defined<typename T::core_specialization>::type
       > {
    enum { value = true };
};

template <class SM>
struct core_traits<SM, typename boost::disable_if<core_specialized<SM> >::type> {
    typedef typename SM::state         state;
    typedef typename SM::transition    transition;
    typedef typename SM::token_type    token_type;
    typedef typename SM::weight_type   weight_type;
    
    typedef typename SM::transition_iterator transition_iterator;
    
    typedef std::pair<transition_iterator,transition_iterator>
            transition_iterator_pair;
                     
    static  token_type epsilon() { return SM::epsilon(); }
};

///////////////////////////////////////////////////////////////////////////////

template <class SM, class AT = void, class Extend = void >
struct machine_class_traits {};

template <class SM, class E = void> 
struct machine_class_specialized {
    enum { value = false };
};

template <class SM> 
struct machine_class_specialized<
           SM
         , typename enable_if_defined<typename SM::machine_class_specialization>::type
       > {
    enum { value = true };
};

template <class SM>
struct machine_class_traits< 
           SM
         , typename boost::enable_if<is_acceptor<SM> >::type
         , typename boost::disable_if<machine_class_specialized<SM> >::type
       > {
    typedef typename SM::input_type input_type;
    static input_type input_epsilon() { return SM::input_epsilon(); }
};

template <class SM>
struct machine_class_traits< 
           SM
         , typename boost::enable_if<is_transducer<SM> >::type
         , typename boost::disable_if<machine_class_specialized<SM> >::type
       > {
    typedef typename SM::input_type input_type;
    static input_type input_epsilon() { return SM::input_epsilon(); }
    
    typedef typename SM::output_type output_type;
    static output_type output_epsilon() { return SM::output_epsilon(); }
};

///////////////////////////////////////////////////////////////////////////////

template <class SM>
struct state_machine_traits
: concept_traits<SM>
, core_traits<SM>
, machine_class_traits<SM>
, finite_state_traits<SM>
, invertible_traits<SM> {};

} // namespace machina

#endif // MACHINA_STATE_MACHINE_TRAITS_HPP