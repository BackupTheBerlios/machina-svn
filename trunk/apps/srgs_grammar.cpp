//#define BOOST_SPIRIT_DEBUG
#include "srgs_grammar.hpp"
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>#include <iostream>#include <string>////////////////////////////////////////////////////////////////////////////using namespace std;using namespace boost::spirit;

int main ( )
{
    cout << "/////////////////////////////////////////////////////////\n\n";    cout << "\t\tgrammar parser...\n\n";    cout << "/////////////////////////////////////////////////////////\n\n";    cout << "Type an expression...or [q or Q] to quit\n\n";    srgs_grammar srgs;    //  Our parser    string str;    while (getline(cin, str))    {        if (str[0] == 'q' || str[0] == 'Q')            break;
            
                    file_iterator<char> first(str.c_str());        if (!first)        {            cout << "Unable to open file!\n";            continue;        }        file_iterator<char> last = first.make_end();        parse_info< file_iterator<char> > info = 
        parse(first,last,srgs,comment_p("/*","*/") | comment_p("//") | space_p);        if (info.full)        {            cout << "-------------------------\n";            cout << "Parsing succeeded\n";            cout << "-------------------------\n";        }        else        {            cout << "-------------------------\n";            cout << "Parsing failed\n";            cout << "stopped at: \": " << info.stop << "\"\n";            cout << "-------------------------\n";        }    }    cout << "Bye... :-) \n\n";
     return 0;}
