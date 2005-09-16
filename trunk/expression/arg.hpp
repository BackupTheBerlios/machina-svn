/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_EXPRESSION_ARG_HPP
#define   MACHINA_EXPRESSION_ARG_HPP

#include "../state_machine.hpp"
#include <utility>
#include <list>
#include <iterator>

namespace machina {

////////////////////////////////////////////////////////////////////////////////

template <typename SM>
struct connection_to {
    connection_to( typename state_machine_traits<SM>::token_type  tok,
                   typename state_machine_traits<SM>::weight_type wt,
                   typename state_machine_traits<SM>::state       to ) 
    :              tok(tok), wt(wt), to(to) {}
    
    typename state_machine_traits<SM>::token_type  tok;
    typename state_machine_traits<SM>::weight_type wt;
    typename state_machine_traits<SM>::state       to;
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM>
struct connection_from {
    connection_from(   typename state_machine_traits<SM>::state       from,
                       typename state_machine_traits<SM>::token_type  tok,
                       typename state_machine_traits<SM>::weight_type wt   ) 
    :                  from(from), tok(tok), wt(wt) {}
    typename state_machine_traits<SM>::state       from;
    typename state_machine_traits<SM>::token_type  tok;
    typename state_machine_traits<SM>::weight_type wt;
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM>
struct connection {
    connection( typename state_machine_traits<SM>::token_type  tok,
                typename state_machine_traits<SM>::weight_type wt )
    :           tok(tok), wt(wt) {}
    typename state_machine_traits<SM>::token_type  tok;
    typename state_machine_traits<SM>::weight_type wt;
};

////////////////////////////////////////////////////////////////////////////////

template <typename SM>
class expression_arg
{
public:
    typedef std::list< connection<SM> >      connection_list;
    typedef std::list< connection_to<SM> >   connection_to_list;
    typedef std::list< connection_from<SM> > connection_from_list;
    
    typedef typename connection_list::const_iterator 
                     connect_const_iterator;

    typedef typename connection_list::iterator 
                     connect_iterator;
                     
    typedef typename std::back_insert_iterator< connection_list >
                     connect_insert_iterator;
    
    typedef typename connection_to_list::const_iterator 
                     to_const_iterator;

    typedef typename connection_to_list::iterator 
                     to_iterator;
    
    typedef typename std::back_insert_iterator< connection_to_list >
                     to_insert_iterator;
    
    typedef typename connection_from_list::const_iterator 
                     from_const_iterator;

    typedef typename connection_from_list::iterator 
                     from_iterator;
    
    typedef typename std::back_insert_iterator< connection_from_list >
                     from_insert_iterator;
    
                     
    connect_const_iterator connection_begin() const 
    {return connect_list.begin();}
    
    connect_const_iterator connection_end()   const 
    {return connect_list.end();}

    connect_iterator connection_begin() 
    {return connect_list.begin();}
    
    connect_iterator connection_end() 
    {return connect_list.end();}
    
    connect_insert_iterator connection_inserter()
    {return std::back_inserter(connect_list);}
    
    from_const_iterator from_begin() const 
    {return from_list.begin();}
    
    from_const_iterator from_end()   const 
    {return from_list.end();}

    from_iterator from_begin()
    {return from_list.begin();}
    
    from_iterator from_end()
    {return from_list.end();}
    
    from_insert_iterator from_inserter()
    {return std::back_inserter(from_list);}
    
    to_const_iterator to_begin() const 
    {return to_list.begin();}
    
    to_const_iterator to_end()   const 
    {return to_list.end();}
    
    to_iterator to_begin()
    {return to_list.begin();}
    
    to_iterator to_end()
    {return to_list.end();}
    
    to_insert_iterator to_inserter()
    {return std::back_inserter(to_list);}
    
    void clear()
    {
        connect_list.clear();
        to_list.clear();
        from_list.clear();
    }
    
    void clear_from()       { from_list.clear(); }
    void clear_to()         { to_list.clear(); }
    void clear_connection() { connect_list.clear(); }
    
private:
      connection_list      connect_list;
      connection_to_list   to_list;
      connection_from_list from_list;
};

template <typename Expr>
class expression;

template <typename SM, typename Expr>
SM&
set_equal (SM& sm, const expression<Expr>& expr)
{
    typename state_machine_traits<SM>::state start, end;
    start = start_state(sm);

    bool final_is_set(false);
    
    typename expression_arg<SM>::connect_const_iterator citr, cend;
    typename expression_arg<SM>::to_const_iterator titr, tend;
    typename expression_arg<SM>::from_const_iterator fitr, fend;
    expression_arg<SM> arg;
    expr.apply(sm,arg);
    citr = arg.connection_begin();
    cend = arg.connection_end();
    for(;citr != cend; ++citr) {
        add_transition(sm,start,end,citr->tok,citr->wt);
    }
    titr = arg.to_begin(); tend = arg.to_end();
    for(;titr != tend; ++titr) {
        if (!final_is_set) {
            end = add_state(sm,true);
            final_is_set=true;
        }
        add_transition(sm,start,titr->to,titr->tok,titr->wt);
    }
    fitr = arg.from_begin(); fend = arg.from_end();
    for(;fitr != fend; ++fitr) {
        if (!final_is_set) {
            end = add_state(sm,true);
            final_is_set=true;
        }
        add_transition(sm,fitr->from,end,fitr->tok,fitr->wt);
    }     
    return sm;
}

}; // namespace machina

#endif // MACHINA_EXPRESSION_ARG
