#include <rule_expression.hpp>

//namespace search {
//
//   class rule
//   {
//      std::string name;
//   public:
//      rule(std::string name):name(name){}
//   };
//
//};


int main()
{
   using namespace search;
   using std::cout;
   using std::endl;
   using std::flush;
   
   rule foo("foo");
   
   cout << "testing rule..."<< endl;
   
   cout << foo << endl << flush;
   
   rule bar("bar");
   
   rule_expression expr1 = foo;
   
   cout << expr1 << endl;
   
   /////////////////////////////////////////////////////////////////////////////
   ///
   ///  <rule_expression>
   ///    <concat>
   ///      <ref>foo</ref>
   ///      <kleene>
   ///        <concat>
   ///          <tag>baz</tag>
   ///          <ref>bar</ref>
   ///        </concat>
   ///      </kleene>
   ///    </concat>
   ///  </rule_expression>
   ///
   /////////////////////////////////////////////////////////////////////////////
   rule_expression expr = foo + *(tag_expr("baz") + bar + acceptor_expr("yes"));
   
   cout << expr << endl;
   
   return 0;
}
