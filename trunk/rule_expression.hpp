/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/

#ifndef   MACHINA_RULE_EXPRESSION_HPP
#define   MACHINA_RULE_EXPRESSION_HPP

#include <boost/shared_ptr.hpp>
#include <string>
#include <list>
#include <iostream>

namespace search {

   class rule
   {
      std::string name;
      friend std::ostream& operator << (std::ostream& os, const rule& rule);
   public:
      rule(std::string name):name(name){}
   };
   
   std::ostream& operator << (std::ostream& os, const rule& rule)
   {
      return os << rule.name;
   }

   class rule_expression
   {
      /// no-frills rtti
      enum expression_op {REF,TAG,ACCEPT,EPSILON,OR,KLEENE,CAT};
      
      struct node 
      {
         expression_op op;
         node(expression_op op):op(op){}
         virtual std::ostream& print(std::ostream& os) const
         {return os;}
         virtual ~node(){}
      };
      
      friend std::ostream& operator << (std::ostream& os, const node& nd)
      {
         return nd.print(os);
      }
      
      typedef std::list< boost::shared_ptr<node> > operand_list;
      
      struct ref_node : public node
      {
         ref_node(const rule& ref):node(REF),ref(ref){}
         const rule& ref;
         virtual std::ostream& print(std::ostream& os) const
         {
            return os << "<ref>" << ref << "</ref>"; 
         }
         virtual ~ref_node(){}
      };
      
      template <typename TagT>
      struct tag_node : public node
      {
         typedef TagT tag_type;
         tag_node(const tag_type& tag):node(TAG),tag(tag){}
         tag_type tag;
         virtual std::ostream& print(std::ostream& os) const
         {
            return os << "<tag>" << tag << "</tag>"; 
         }
         virtual ~tag_node(){}
      };
      
      template <typename AcceptorT>
      struct acceptor_node : public node
      {
         typedef AcceptorT acceptor_type;
         acceptor_type accept;
         acceptor_node(acceptor_type accept):node(ACCEPT),accept(accept){}
         virtual std::ostream& print(std::ostream& os) const
         {
            return os << "<accept>" << accept << "</accept>"; 
         }
         virtual ~acceptor_node(){}
      };
      
      struct epsilon_node : public node
      {
         epsilon_node():node(EPSILON){}
         virtual std::ostream& print(std::ostream& os) const
         {
            return os << "<epsilon/>"; 
         }
      };
      
      struct or_node : public node
      {
         or_node():node(OR){}
         std::list< boost::shared_ptr<node> > operands;
         virtual std::ostream& print(std::ostream& os) const
         {
            os << "<or>";
            operand_list::const_iterator itr = operands.begin();
            operand_list::const_iterator end = operands.end();
            for (; itr != end; ++itr) (*itr)->print(os);
            os << "</or>";
            return os;
         }
         virtual ~or_node(){}
      };
      
      struct kleene_node : public node
      {
         kleene_node(boost::shared_ptr<node> operand):node(KLEENE),
                                                      operand(operand){}
         boost::shared_ptr<node> operand;
         virtual std::ostream& print(std::ostream& os) const
         {
            os << "<kleene>" << *operand << "</kleene>";
         }
         virtual ~kleene_node(){}
      };
      
      struct cat_node : public node
      {
         std::list< boost::shared_ptr<node> > operands;
         cat_node():node(CAT){}
         virtual std::ostream& print(std::ostream& os) const
         {
            os << "<concat>";
            operand_list::const_iterator itr = operands.begin();
            operand_list::const_iterator end = operands.end();
            for (; itr != end; ++itr) (*itr)->print(os);
            os << "</concat>";
            return os;
         }
         virtual ~cat_node(){}
      };
      
      boost::shared_ptr<node> root;
      
   public:
      template <typename AcceptorT>
      friend rule_expression acceptor_expr (AcceptorT acc)
      {
         boost::shared_ptr<node> root(new acceptor_node<AcceptorT>(acc));
         return rule_expression(root);
      }
      
      template <typename TagT>
      friend rule_expression tag_expr (TagT tag)
      {
         boost::shared_ptr<node> root(new tag_node<TagT>(tag));
         return rule_expression(root);
      }
      
      friend rule_expression epsilon_expr ()
      {  
         boost::shared_ptr<node> root(new epsilon_node);
         return rule_expression(root);
      }
      
      rule_expression(const rule& ref):root(new ref_node(ref)){}
      
      friend rule_expression operator | (const rule_expression& expr1,
                                         const rule_expression& expr2)
      {
         boost::shared_ptr<or_node> new_root(new or_node);
         new_root->operands.push_back(expr1.root);
         new_root->operands.push_back(expr2.root);
         return rule_expression(new_root);
      }
      
      rule_expression operator * ( )
      {
         if ( root->op == KLEENE || root->op == EPSILON ) {
            return rule_expression(*this);
         }
         else {
            boost::shared_ptr<node> n(new kleene_node(root));
            return rule_expression(n);
         }
      }
      
      friend rule_expression operator + (const rule_expression& expr1,
                                         const rule_expression& expr2)
      {
         if (expr1.root->op != EPSILON && expr2.root->op != EPSILON) {
            boost::shared_ptr<cat_node> parent(new cat_node);
            parent->operands.push_back(expr1.root);
            parent->operands.push_back(expr2.root);
            return rule_expression(parent);
         }
         else if (expr1.root->op != EPSILON) {
            return rule_expression(expr1.root);
         }
         else return rule_expression(expr2.root); 
      }
   
   friend std::ostream& operator << (std::ostream& os, 
                                     const rule_expression& expr)
   {
      return os << "<rule_expression>" << *(expr.root) << "</rule_expression>";
   }
      
   private:
      rule_expression(boost::shared_ptr<node> root):root(root){}         
   };

}; // namespace search

#endif // RULE_EXPRESSION_HPP
