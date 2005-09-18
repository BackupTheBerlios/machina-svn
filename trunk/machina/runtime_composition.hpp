/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_RUNTIME_COMPOSITION_HPP
#define   MACHINA_RUNTIME_COMPOSITION_HPP

#include "state_machine.hpp"
#include <boost/iterator/iterator_traits.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/call_traits.hpp>

namespace machina {

template <typename InputST, typename OutputSM>
class runtime_composition;

namespace detail {

////////////////////////////////////////////////////////////////////////////////

template <typename OuterIterator, typename InnerIterator>
class product_iterator
: public 
  boost::iterator_facade <
    product_iterator<OuterIterator, InnerIterator>,
    boost::tuple<typename boost::iterator_value<OuterIterator>::type,
                 typename boost::iterator_value<InnerIterator>::type>,
    boost::forward_traversal_tag,
    boost::tuple<typename boost::iterator_reference<OuterIterator>::type,
                 typename boost::iterator_reference<InnerIterator>::type> > {
                      
    product_iterator(OuterIterator outer_pos,
                     InnerIterator inner_begin, 
                     InnerIterator inner_pos, 
                     InnerIterator inner_end)
    : outer_pos(outer_pos),  
      inner_begin(inner_begin), inner_pos(inner_pos), inner_end(inner_end) {}
    
    bool equal(const product_iterator& other) const { 
        return inner_pos == other.inner_pos && outer_pos == other.outer_pos; 
    }  
    
    void increment() {
        ++inner_pos;
        if (inner_pos == inner_end) {
            inner_pos = inner_begin;
            ++outer_pos;
        }
    }
    
    reference dereference() const {
        return reference(*inner_pos, *outer_pos);
    }
                    
private:
    OuterIterator outer_pos;
    InnerIterator inner_begin, inner_pos, inner_end;
};

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina::detail

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
class runtime_composition {
public:
    typedef typename state_machine_traits<OuterSM>::machine_class machine_class;
    typedef typename state_machine_traits<InnerST>::input_type    input_type;
    typedef typename state_machine_traits<InnerST>::token_type    token_type;
    //typedef typename state_machine_traits<OuterSM>::output_type   output_type;
    typedef typename state_machine_traits<InnerST>::weight_type   weight_type;
    
    typedef typename state_machine_traits<InnerST>::state inner_state;
    typedef typename state_machine_traits<OuterSM>::state outer_state;    
    
    typedef typename state_machine_traits<InnerST>::transition 
                     inner_transition;
    typedef typename state_machine_traits<OuterSM>::transition 
                     outer_transition;
                     
    typedef typename state_machine_traits<InnerST>::transition_iterator
                     inner_transition_iterator;
    typedef typename state_machine_traits<OuterSM>::transition_iterator
                     outer_transition_iterator;
                     
    enum filter_state { move_both, move_inner, move_outer, invalid };
    
    class state {
        state(outer_state outer_s, inner_state inner_s, filter_state filter_s)
        : inner_s(inner_s), outer_s(outer_s),filter_s(filter_s) {}
        
        typename boost::call_traits<inner_state>::reference  inner_s;
        typename boost::call_traits<outer_state>::reference  outer_s;
        filter_state                                filter_s;
        friend class runtime_composition;
    };
    
    struct transition {
        state        from;
        state        to;
        token_type   tok;
        weight_type  wt;
        friend class transition_iterator;
    };
    
    class filter_op {
    public:
        typedef boost::tuple<inner_transition&,outer_transition&> trans_pair;
        bool operator() (const trans_pair& t) {
            switch (from_filter_s) {
            case move_both:
                return case_move_both(t);  break;
            case move_outer:
                return case_move_outer(t); break;
            case move_inner:
                return case_move_inner(t); break;
            default:
                return false; break;
            }
        }
        filter_state filter_state_to() { return to_filter_s; }
    private:
        bool case_move_both (const trans_pair& t);
        bool case_move_inner(const trans_pair& t);
        bool case_move_outer(const trans_pair& t);
        filter_state from_filter_s, to_filter_s;
        runtime_composition const & sm;
        friend class transition_iterator;
    };
    
    class transition_iterator {
        transition const & dereference() { return current; }
        void increment() {
            ++itr;
            if (itr.base() != itr.end()) {
                switch (filter.to_filter_s) {
                case move_both:
                    case_move_both(); break;
                case move_outer:
                    case_move_outer(); break;
                case move_inner:
                    case_move_inner(); break;
                default:
                    assert(false && 
                           "to_filter_s should be valid: filter true");
                }
            }
        }
        void case_move_both();
        void case_move_inner();
        void case_move_outer();
        boost::call_traits<runtime_composition> sm;
        filter_state                            filter_s;
        friend class runtime_composition;
        typedef boost::filter_iterator<
                       filter_op,
                       detail::product_iterator< outer_transition_iterator,
                                                 inner_transition_iterator > 
                > iterator_type;
        filter_op     filter;
        iterator_type itr;
        transition    current;
    };
    
    state start_state() 
    { return state(start_state(outer_st),start_state(inner_st),move_both); }
    
    bool is_final(state s)
    { return is_final(inner_st,s.inner_s) && is_final(outer_st, s.outer_s); }
       
    runtime_composition(typename boost::call_traits<OuterSM>::param_type out_sm,
                        typename boost::call_traits<InnerST>::param_type in_st)
    : inner_st(in_st),outer_sm(out_sm) {}
private:
    typename boost::call_traits<InnerST>::value_type inner_st;
    typename boost::call_traits<OuterSM>::value_type outer_sm;
};

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina

#include "impl/runtime_composition.ipp"
#endif // MACHINA_COMPOSITION_HPP