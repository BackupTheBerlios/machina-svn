//#define BOOST_SPIRIT_DEBUG
#include "srgs_grammar.hpp"
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>

int main ( )
{
    cout << "/////////////////////////////////////////////////////////\n\n";
            
            
        parse(first,last,srgs,comment_p("/*","*/") | comment_p("//") | space_p);
 