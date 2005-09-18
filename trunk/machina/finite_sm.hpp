
namespace machina {

typedef <typename Input = std::string, typename Weight = logprob>
class finite_sm {
public:
    typedef token<Input> token_type;
    typedef Weight       weight_type;
    class state;
private:
    class transition {
        boost::weak_ptr<state> m_from;
        boost::weak_ptr<state> m_to;
        token_type             m_tok;
        weight_type            m_wt;
        friend class finite_sm;
        friend struct trans_partial_order;
    };
    
    struct trans_partial_order {
        bool operator () (const transition& t1, const transition& t2) const {
            return t1.m_tok < t2.m_tok;
        }
    };
    
    typedef std::multiset<transition, transition_partial_order>
            transition_set;
    
    class state {
        transition_set m_trans_set;
        friend class finite_sm;
    };
    
    std::list< boost::shared_ptr<state> > m_state_list;
};

}; // namespace machina