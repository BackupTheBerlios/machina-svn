namespace machina {

template <typename OuterSM, typename InnerST>
bool 
runtime_composition<OuterSM, InnerST>::
filter_op::case_move_both(const trans_pair& t) {
    if (get_output(sm, get<0>(t)) == 
        state_machine_traits<InnerST>::output_epsilon()) {
        if (get_input(sm, get<0>(t)) ==
            state_machine_traits<OuterSM>::input_epsilon()) {
            to_filter_s = move_both;
        } else { to_filter_s = move_inner; }
        return true;
    } else if ( get_input(sm, get<1>) ==
              state_machine_traits<OuterSM>::input_epsilon() ) {
        to_filter_s = move_outer;
    } else if ( get_output(sm, get<0>(t)) == 
                get_input(sm, get<1>(t)) ) {
        to_filter_s = move_both;
        return true;
    } else { return false; }
}

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
bool 
runtime_composition<OuterSM, InnerST>::
filter_op::case_move_inner (const trans_pair& t) {
    if (get_output(sm, get<0>(t)) == 
        state_machine_traits<InnerST>::output_epsilon()) {
        to_filter_s = move_inner;
        return true;
    } else if ( get_output(sm, get<0>(t)) == get_input(sm, get<1>(t)) ) {
        to_filter_s = move_both;
        return true;
    } else { return false; }
}

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
bool 
runtime_composition<OuterSM, InnerST>::
filter_op::case_move_outer (const trans_pair& t) {
    if (get_input(sm, get<1>(t)) == 
        state_machine_traits<OuterSM>::inner_epsilon()) {
        to_filter_s = move_outer;
        return true;
    } else if ( get_output(sm, get<0>(t)) == get_input(sm, get<1>(t)) ) {
        to_filter_s = move_both;
        return true;
    } else { return false; }
}

template <typename O, typename I>
typename boost::enable_if<
    is_transducer<O>,
    typename state_machine_traits< runtime_composition<O,I> >::token_type 
>::type 
_compose(typename boost::call_traits<typename state_machine_traits<O>::token_type>::param_type
         outtok,
         typename boost::call_traits<typename state_machine_traits<I>::token_type>::param_type
         intok) {
         
    return state_machine_traits< runtime_composition<O,I> >::token_type
           (get<0>(intok),get<1>(outtok));
}

template <typename Out, typename In>
typename boost::enable_if<
    is_acceptor<Out>,
    typename state_machine_traits< runtime_composition<Out,In> >::token_type 
>::type 
_compose(typename boost::call_traits<typename state_machine_traits<Out>::token_type>::param_type 
         outtok,
         typename boost::call_traits<typename state_machine_traits<In>::token_type>::param_type
         intok) {
         
    return state_machine_traits< runtime_composition<Out,In> >::token_type
           (get<0>(intok));
}

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
void runtime_composition<OuterSM, InnerST>::
transition_iterator::case_move_both() {
    current.to  = state(to(filter.sm.outer_sm(),get<0>(*itr)),
                        to(filter.sm.inner_st(),get<1>(*itr)),
                        filter.to_filter_s);
    current.wt  = get_weight(filter.sm.outer_sm(),get<0>(*itr))
                  * get_weight(filter.sm.inner_st(),get<1>(*itr));
    current.tok = _compose<OuterSM,InnerST>(
                      get_token(filter.sm.outer_sm(),get<0>(*itr)),
                      get_token(filter.sm.inner_st(),get<1>(*itr))
                  );
}

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
void runtime_composition<OuterSM, InnerST>::
transition_iterator::case_move_inner() {
    current.to  = state(from(filter.sm.outer_sm(),get<0>(*itr)),
                        to  (filter.sm.inner_st(),get<1>(*itr)),
                        filter.to_filter_s);
    current.wt  = get_weight(filter.sm.inner_st(),get<1>(*itr));
    current.tok = _compose<OuterSM,InnerST>(
                      state_machine_traits<OuterSM>::epsilon(),
                      get_token(filter.sm.inner_st(),get<1>(*itr))
                  );    
}

////////////////////////////////////////////////////////////////////////////////

template <typename OuterSM, typename InnerST>
void runtime_composition<OuterSM, InnerST>::
transition_iterator::case_move_outer() {
    current.to  = state(to  (filter.sm.outer_sm(),get<0>(*itr)),
                        from(filter.sm.inner_st(),get<1>(*itr)),
                        filter.to_filter_s);
    current.wt  = get_weight(filter.sm.outer_sm(),get<0>(*itr));
    current.tok = _compose<OuterSM,InnerST>(
                      get_token(filter.sm.outer_sm(),get<0>(*itr)),
                      state_machine_traits<InnerST>::epsilon()
                  );    
}

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina
