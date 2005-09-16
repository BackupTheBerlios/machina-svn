#ifndef   MACHINA_DETAIL_PARTIAL_ORDER_HPP
#define   MACHINA_DETAIL_PARTIAL_ORDER_HPP

#include  <boost/graph/adjacency_list.hpp>
#include  <set>
#include  <boost/multi_index_container.hpp>

namespace machina { namespace detail {

    struct transition_data_tag { typedef boost::edge_property_tag kind; };
    struct state_data_tag { typedef boost::vertex_property_tag kind; };      

    template <typename TransT>
    struct partial_order
    : public std::binary_function<TransT,TransT,bool> {
        bool operator() (const TransT & d1, const TransT & d2) const {
            return boost::get(transition_data_tag(),d1).pos(
                       boost::get(transition_data_tag(),d2)) ;
        }
    };
    struct porder_multiset_tag {};
}; }; // namespace machina::detail

namespace boost {
    template <class ValueT>
    struct container_gen<machina::detail::porder_multiset_tag, ValueT> {
//        typedef multi_index_container<
//                    ValueT, 
//                    multi_index::indexed_by< 
//                        multi_index::ordered_non_unique < 
//                            multi_index::identity<ValueT>,
//                            machina::detail::partial_order<ValueT>
//                        > 
//                    >
//                > type;
        typedef std::multiset< ValueT, machina::detail::partial_order<ValueT> >
                type;
    };
    template <>
    struct parallel_edge_traits<machina::detail::porder_multiset_tag> {
        typedef allow_parallel_edge_tag type;
    };
};

#endif // MACHINA_DETAIL_PARTIAL_ORDER_HPP
