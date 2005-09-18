#ifndef   MACHINA_FINITE_ST_HPP
#define   MACHINA_FINITE_ST_HPP

#include  <boost/multi_index_container.hpp>
#include  <boost/functional/hash/hash.hpp>
#include  <boost/multi_index/hashed_index.hpp>
#include  <boost/multi_index/ordered_index.hpp>
#include  <boost/shared_ptr.hpp>
#include  <boost/iterator/iterator_facade.hpp>

#include  <list>
#include  <string>
#include  <memory>

#include  "state_machine.hpp"
#include  "token.hpp"

namespace machina {

template < class InT    = std::string
         , class OutT   = std::string
         , class WtT    = double
         , class AllocT = std::allocator< boost::tuple<InT,OutT> >
         >
class finite_st {
    struct state_info;
    class  trans_info;
public:
    typedef token<InT>                           input_type;
    typedef token<OutT>                          output_type;
    typedef WtT                                  weight_type; 
    typedef boost::tuple<input_type,output_type> token_type;
    
    typedef state_info* state;
    typedef trans_info* transition;
    
    typedef transducer_tag machine_class;
private:
    ////////////////////////////////////////////////////////////////////////////
    class trans_info {    
        state_info* m_from;
        state_info* m_to;
        token_type  m_tok;
        weight_type m_weight;
    public:
        trans_info( state_info* f 
                  , state_info* t
                  , token_type  const& tok
                  , weight_type const& wt
                  )
        : m_from(f)
        , m_to(t)
        , m_tok(tok)
        , m_weight(wt) {}
        token_type  const& token()  const {return m_tok;}
        input_type  const& input()  const {return boost::get<0>(m_tok);}
        output_type const& output() const {return boost::get<1>(m_tok);}
        weight_type const& weight() const {return m_weight;}
        state_info*        from()   const {return m_from;}
        state_info*        to()     const {return m_to;}
    };
    
    typedef boost::shared_ptr<trans_info> trans_ptr;
    
    ////////////////////////////////////////////////////////////////////////////
    struct weight_extractor {
        typedef weight_type result_type;
        weight_type const& operator()(trans_ptr const& t) const {
            return t->weight();
        }
    };
    struct input_extractor {
        typedef input_type result_type;
        input_type const& operator()(trans_ptr const& t) const {
            return t->input();
        } 
    };
    struct output_extractor {
        typedef output_type result_type;
        output_type const& operator()(trans_ptr const& t) const {
            return t->output();
        }
    };
    ////////////////////////////////////////////////////////////////////////////
    struct input_tag{};
    struct output_tag{};
    struct weight_tag{};
    
    struct trans_index_list 
    : boost::multi_index::indexed_by<
          boost::multi_index::hashed_non_unique<
            boost::multi_index::tag<input_tag>
          , input_extractor
          >
        , boost::multi_index::hashed_non_unique<
            boost::multi_index::tag<output_tag>
          , output_extractor
          >
        , boost::multi_index::ordered_non_unique<
            boost::multi_index::tag<weight_tag>
          , weight_extractor
          >
      > {};
    
    struct state_info {
        typedef boost::multi_index_container<
          trans_ptr
        , trans_index_list
        , typename AllocT::template rebind<trans_ptr>::other
        > trans_container_type;
        typedef typename trans_container_type::iterator trans_itr_type;
        typedef typename boost::multi_index::index_iterator<
                             trans_container_type,
                             weight_tag>::type wt_trans_itr_type;
        typedef typename boost::multi_index::index_iterator<
                             trans_container_type,
                             output_tag>::type inv_trans_itr_type;
        state_info(bool is_final):is_final(is_final) {}
        trans_container_type trans_container;
        bool                 is_final;
    };
    
    typedef boost::shared_ptr<state_info> state_ptr;
    typedef std::list<
               state_ptr
            ,  typename AllocT::template rebind<state_ptr>::other
            > state_container_type;
    state_container_type my_state_container;
    state_ptr my_start_state;
    
    typedef typename AllocT::template rebind<state_info>::other 
            state_alloc_type;
    typedef typename AllocT::template rebind<trans_info>::other 
            trans_alloc_type;
            
    trans_alloc_type trans_alloc;
    state_alloc_type state_alloc;
    
    struct state_destructor {
        state_destructor(state_alloc_type& d) :d(d) {}
        void operator()(state_info* s) const {
            d.destroy(s);
            d.deallocate(s,1);
        }
        state_alloc_type& d;
    };
    struct trans_destructor{
        trans_destructor(trans_alloc_type& d) :d(d) {}
        void operator()(trans_info* s) const {
            d.destroy(s);
            d.deallocate(s,1);
        }
        trans_alloc_type& d;
    };
    
    // creation method for state_info objects
    state_ptr state_create(bool isfinal) {
        state_info* p(state_alloc.allocate(1));
        try {
            ::new(p) state_info(isfinal);
        } catch (std::exception& e) {state_alloc.deallocate(p,1); throw;}
        return state_ptr(p,state_destructor(state_alloc));
    }
    // creation method for trans_info objects
    trans_ptr trans_create( state from
                          , state to
                          , token_type  const& tok
                          , weight_type const& wt ) {
        trans_info* p(trans_alloc.allocate(1));
        try {
            ::new(p) trans_info(from, to, tok, wt);
        } catch (std::exception& e) {trans_alloc.deallocate(p,1); throw;}
        return trans_ptr(p,trans_destructor(trans_alloc));
    }
    template <class IteratorT, class ElemT> 
    class p_iterator 
    : public boost::iterator_facade<
               p_iterator<IteratorT, ElemT>
             , ElemT*
             , boost::bidirectional_traversal_tag
             , ElemT*
             > {
        typedef IteratorT itr_type;
        typedef ElemT*    ref_type;
        itr_type mys;
    public:
        p_iterator() {}
        explicit p_iterator(itr_type s)
        : mys(s) {}
    private:
        friend class boost::iterator_core_access;    
        ref_type dereference() const { return mys->get(); }
        void     increment() { ++mys; }
        void     decrement() { --mys; }
        bool     equal(p_iterator const& o) const { return mys == o.mys; }
    };
public:
    typedef p_iterator<typename state_info::trans_itr_type,trans_info> 
            transition_iterator;
    typedef p_iterator< typename state_container_type::const_iterator
                      , state_info
                      > state_iterator;
    typedef p_iterator<typename state_info::wt_trans_itr_type,trans_info>
            wsorted_transition_iterator;
    typedef p_iterator<typename state_info::inv_trans_itr_type,trans_info>
            inverse_transition_iterator;
    finite_st() {
        my_start_state = state_create(false);
        my_state_container.push_back(my_start_state);
    }
    
    state start_state() const {return my_start_state.get();}
    
    /// if true, the state s is in a final, or accepting state.
    bool  is_final(state s) const {return s->is_final;} 
    void  set_final(state s, bool is_final) {s->is_final = is_final;}
    
    state add_state(bool isfinal=false) {
        state_ptr p(state_create(isfinal));
        my_state_container.push_back(p);
        return p.get();
    }
    
    /// returns a range over all the states in this state machine.
    std::pair<state_iterator,state_iterator> states() const {
        return std::make_pair( state_iterator(my_state_container.begin())
                             , state_iterator(my_state_container.end()) );
    }
    
    transition add_transition( state from
                             , state to
                             , token_type  const& tok
                             , weight_type const& wt ) {
        typename state_info::trans_itr_type ti = 
            from->trans_container.insert(trans_create( from
                                                     , to
                                                     , tok
                                                     , wt ) ).first;
        return ti->get();
    }
    
    transition add_transition( state from
                             , state to
                             , input_type  const& in
                             , output_type const& out
                             , weight_type const& wt ) {
        return add_transition(from, to, token_type(in,out), wt);
    }
    
    transition add_transition( state from
                             , state to
                             , input_type const& in
                             , weight_type const& wt ) {
        return add_transition(from, to, token_type(in,output_type()), wt);
    }
    
    void remove_transition( transition t) {
        typename state_info::trans_itr_type itr, end;
        itr = t->from()->trans_container.begin();
        end = t->from()->trans_container.end();
        for (;itr != end; ++itr) if (itr->get() == t) {
            t->from()->trans_container.erase(itr);
            break;
        }
    }
    
    void remove_state(state s) {
        typename state_container_type::iterator itr, end;
        itr = my_state_container.begin();
        end = my_state_container.end();
        for (;itr != end; ++itr) if (itr->get() == s) {
            my_state_container.erase(itr);
            break;
        }
    }
    
    std::pair<transition_iterator,transition_iterator>
    transitions(state s) const {
        return std::make_pair( transition_iterator(s->trans_container.begin())
                             , transition_iterator(s->trans_container.end())
                        );
    }
    std::pair< wsorted_transition_iterator
             , wsorted_transition_iterator >
    transitions_by_weight(state s) const {
        return std::make_pair( 
                   wsorted_transition_iterator(
                   boost::multi_index::get<weight_tag>(s->trans_container).begin()
                      // s->trans_container.template get<weight_tag>().begin()
                   )
               ,   wsorted_transition_iterator(
                   boost::multi_index::get<weight_tag>(s->trans_container).end()
                       //s->trans_container.template get<weight_tag>().end()
                   )
               );
    }
    std::pair< inverse_transition_iterator
             , inverse_transition_iterator >
    inverse_transitions(state s) const {
        return std::make_pair( 
                   inverse_transition_iterator(
                   boost::multi_index::get<output_tag>(s->trans_container).begin()
                       //s->trans_container.template get<output_tag>().begin()
                   )
               ,   inverse_transition_iterator(
                   boost::multi_index::get<output_tag>(s->trans_container).end()
                      // s->trans_container.template get<output_tag>().end()
                   )
               );
    }    
    std::pair<transition_iterator,transition_iterator>
    transitions(state s, input_type const& in) const {
        std::pair< typename state_info::trans_itr_type
                 , typename state_info::trans_itr_type> p =
                                            s->trans_container.equal_range(in);
        return std::make_pair( transition_iterator(p.first)
                             , transition_iterator(p.second)
                             );
    }
    std::pair<inverse_transition_iterator,inverse_transition_iterator>
    inverse_transitions(state s, output_type const& out) const {
        std::pair< typename state_info::inv_trans_itr_type
                 , typename state_info::inv_trans_itr_type> p =
        boost::multi_index::get<output_tag>(s->trans_container).equal_range(out);
            //s->trans_container.template get<output_tag>().equal_range(out);
        return std::make_pair( inverse_transition_iterator(p.first)
                             , inverse_transition_iterator(p.second)
                             );
    }

    input_type   get_input(transition t) const { return t->input();  }
    output_type get_output(transition t) const { return t->output(); }
    token_type   get_token(transition t) const { return t->token();  }
    weight_type get_weight(transition t) const { return t->weight(); }
    state               to(transition t) const { return t->to();     }
    state             from(transition t) const { return t->from();   }
    
    static input_type  input_epsilon()  { return input_type::epsilon();  }
    static output_type output_epsilon() { return output_type::epsilon(); } 
    static token_type  epsilon() {
        return token_type(input_epsilon(),
                          output_epsilon());
    }
                             
};

} // namespace machina

#endif // MACHINA_FINITE_ST_HPP