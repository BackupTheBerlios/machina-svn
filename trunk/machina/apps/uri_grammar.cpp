#define BOOST_SPIRIT_DEBUG
#include <boost/spirit/core.hpp>#include <iostream>#include <string>////////////////////////////////////////////////////////////////////////////using namespace std;using namespace boost::spirit;

struct printer
{
    printer(const string& name):name(name){}
    
    void operator() (const char* begin, const char* end) const
    {
 //       cout << "<" << name << ">"
 //            << string(begin,end) 
 //            << "</" << name << ">" << endl;
    }
    
    string name;
};

struct uri_grammar : public grammar<uri_grammar>{    template <typename ScannerT>    struct definition    {

        rule<typename lexeme_scanner<ScannerT>::type> uri_reference;
        rule<typename lexeme_scanner<ScannerT>::type> 
                                             absolute_uri,
                                             relative_uri,
                                             hier_part,
                                             opaque_part,
                                             uric_no_slash,
                                             net_path,
                                             abs_path,
                                             rel_path,
                                             rel_segment,
                                             scheme,
                                             authority,
                                             reg_name,
                                             server,
                                             userinfo,
                                             hostport,
                                             host,
                                             hostname,
                                             domainlabel,
                                             toplabel,
                                             ipv4_address,
                                             port,
                                             //path,
                                             path_segments,
                                             segment,
                                             param,
                                             pchar,
                                             query,
                                             fragment,
                                             uric,
                                             reserved,
                                             unreserved,
                                             mark,
                                             escaped,
                                             alnumdash;

        definition(uri_grammar const& self)  
        {
            ////////////////////////////////////////////////////////////////////////////////
            ///
            ///   uri_reference = [ absolute_uri | relative_uri ] [ "#" fragment ]            ///   absolute_uri  = scheme ":" ( hier_part | opaque_part )
            ///   relative_uri  = ( net_path | abs_path | rel_path ) [ "?" query ]
            ///
            ////////////////////////////////////////////////////////////////////////////////
            uri_reference = lexeme_d[
                             !( absolute_uri | relative_uri ) >> !( '#' >> fragment )
                            ][printer("uri_reference")];
            absolute_uri  = lexeme_d[
                             (scheme >> ch_p(':') >> ( hier_part | opaque_part ))
                            ][printer("absolute_uri")];
            relative_uri  = lexeme_d[
                             (( net_path | abs_path | rel_path ) >> !( ch_p('?') >> query ))
                            ][printer("relative_uri")];
            
            ////////////////////////////////////////////////////////////////////////////////            ///            ///   hier_part     = ( net_path | abs_path ) [ "?" query ]            ///   opaque_part   = uric_no_slash *uric            ///
            ////////////////////////////////////////////////////////////////////////////////
            hier_part   = lexeme_d[
                           (( net_path | abs_path ) >> !( ch_p('?') >> query ))
                          ][printer("hier_part")];
            opaque_part = lexeme_d[
                           (uric_no_slash >> *uric)
                          ][printer("opaque_part")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |            ///                   "&" | "=" | "+" | "$" | ","            ///
            ////////////////////////////////////////////////////////////////////////////////
            uric_no_slash = lexeme_d[
                             (unreserved  | escaped | ch_p(';') | ch_p('?') | ch_p(':') | 
                             ch_p('@') | ch_p('&') | ch_p('=') | ch_p('+') | ch_p('$') | 
                             ch_p(','))
                            ][printer("uric_no_slash")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   net_path      = "//" authority [ abs_path ]            ///   abs_path      = "/"  path_segments            ///   rel_path      = rel_segment [ abs_path ]            ///
            ////////////////////////////////////////////////////////////////////////////////
            net_path = lexeme_d[(str_p("//") >> authority >> !abs_path)][printer("net_path")];            abs_path = lexeme_d[(ch_p('/') >> path_segments)][printer("abs_path")];            rel_path = lexeme_d[(rel_segment >> !abs_path)][printer("rel_path")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   rel_segment   = 1*( unreserved | escaped |            ///                       ";" | "@" | "&" | "=" | "+" | "$" | "," )            ///
            ////////////////////////////////////////////////////////////////////////////////
            rel_segment = lexeme_d[
                            (+( unreserved | escaped  | ch_p(';') | ch_p('@') | ch_p('&') | 
                            ch_p('=') | ch_p('+') | ch_p('$') | ch_p(',') ) )
                          ][printer("rel_segment")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   scheme        = alpha *( alpha | digit | "+" | "-" | "." )            ///
            ////////////////////////////////////////////////////////////////////////////////
            scheme = lexeme_d[
                       alpha_p >> *( alpha_p | digit_p | ch_p('+') | ch_p('-') | ch_p('.') )
                     ][printer("scheme")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   authority     = server | reg_name            ///
            ////////////////////////////////////////////////////////////////////////////////
            authority = lexeme_d[server | reg_name][printer("authority")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   reg_name      = 1*( unreserved | escaped | "$" | "," |            ///                       ";" | ":" | "@" | "&" | "=" | "+" )            ///
            ////////////////////////////////////////////////////////////////////////////////
            reg_name = lexeme_d[
                          +( unreserved | escaped  | ch_p('$') | ch_p(',') | ch_p(';') | 
                          ch_p(':') | ch_p('@') | ch_p('&') | ch_p('=') | ch_p('+') )
                       ][printer("reg_name")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   server        = [ [ userinfo "@" ] hostport ]            ///   userinfo      = *( unreserved | escaped |            ///                      ";" | ":" | "&" | "=" | "+" | "$" | "," )            ///
            ////////////////////////////////////////////////////////////////////////////////
            server   = lexeme_d[
                          !( !( userinfo >> ch_p('@') ) >> hostport )
                       ][printer("server")];
            userinfo = lexeme_d[
                          *( unreserved | escaped  | ch_p('$') | ch_p(',') | ch_p(';') | 
                          ch_p(':') | ch_p('&') | ch_p('=') | ch_p('+') )
                       ][printer("userinfo")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   hostport      = host [ ":" port ]            ///   host          = hostname | ipv4_address            ///   hostname      = *( domainlabel "." ) toplabel [ "." ]            ///   domainlabel   = alphanum | alphanum *( alphanum | "-" ) alphanum            ///   toplabel      = alpha | alpha *( alphanum | "-" ) alphanum            ///   ipv4_address  = 1*digit "." 1*digit "." 1*digit "." 1*digit            ///   port          = *digit
            ///   alnumdash     = alphanum alnumdash | +dash alphanum alnumdash | NULL             ///
            ////////////////////////////////////////////////////////////////////////////////
            hostport     = lexeme_d[host >> !( ch_p(':') >> port ) ][printer("hostport")];
                        host         = lexeme_d[hostname | ipv4_address][printer("host")];
                        hostname     = lexeme_d[
                              *( domainlabel >> ch_p('.') ) >> toplabel >> !ch_p('.')
                           ][printer("hostname")];
                                       domainlabel  = lexeme_d[alnum_p >> alnumdash][printer("domainlabel")];
                        toplabel     = lexeme_d[alpha_p >> alnumdash][printer("toplabel")];
                        ipv4_address = lexeme_d[
                              +digit_p >> ch_p('.') >> +digit_p >> ch_p('.') >> 
                              +digit_p >> ch_p('.') >> +digit_p
                           ][printer("ipv4_address")];
                                       port         = lexeme_d[*digit_p][printer("port")];
            
            alnumdash    = lexeme_d[
                              (alnum_p >> alnumdash)               | 
                              (+ch_p('-') >> alnum_p >> alnumdash) | 
                              epsilon_p
                           ];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   path          = [ abs_path | opaque_part ]            ///   path_segments = segment *( "/" segment )            ///   segment       = *pchar *( ";" param )            ///   param         = *pchar            ///   pchar         = unreserved | escaped |            ///                   ":" | "@" | "&" | "=" | "+" | "$" | ","            ///
            ////////////////////////////////////////////////////////////////////////////////
            //path          = (!( abs_path | opaque_part ))[printer("path")];            path_segments = lexeme_d[segment >> *( "/" >> segment )][printer("path_segments")];
                        segment       = lexeme_d[*pchar >> *( ";" >> param )][printer("segment")];
                        param         = lexeme_d[*pchar][printer("param")];
                        pchar         = lexeme_d[
                               unreserved | escaped | ":" | "@" | "&" |
                               "=" | "+" | "$" | ","
                            ];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   query         = *uric            ///
            ////////////////////////////////////////////////////////////////////////////////
            query = lexeme_d[*uric][printer("query")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   fragment      = *uric            ///
            ////////////////////////////////////////////////////////////////////////////////
            fragment = lexeme_d[*uric][printer("fragment")];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///
            ///   uric          = reserved | unreserved | escaped            ///   reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "+" |            ///                   "$" | ","            ///   unreserved    = alphanum | mark            ///   mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |            ///                   "(" | ")"            ///
            ////////////////////////////////////////////////////////////////////////////////
            uric       = lexeme_d[reserved | unreserved | escaped];
            
            reserved   = lexeme_d[
                            ch_p(';') | '/' | '?' | ':' | '@' | '&' | '=' | '+' | '$' | ','
                         ];
                         
            unreserved = lexeme_d[alnum_p | mark];
            
            mark       = lexeme_d[
                            str_p("-") | "_" | "." | "!" | "~" | "*" | "'" | "(" | ")"
                         ];
            
            ////////////////////////////////////////////////////////////////////////////////
            ///            ///   escaped       = "%" hex hex            ///
            ////////////////////////////////////////////////////////////////////////////////
            escaped = lexeme_d[ch_p('%') >> xdigit_p >> xdigit_p];
            
            
            BOOST_SPIRIT_DEBUG_RULE( absolute_uri );
            BOOST_SPIRIT_DEBUG_RULE( relative_uri );
            BOOST_SPIRIT_DEBUG_RULE( hier_part );
            BOOST_SPIRIT_DEBUG_RULE( opaque_part );
            BOOST_SPIRIT_DEBUG_RULE( uric_no_slash );
            BOOST_SPIRIT_DEBUG_RULE( net_path );
            BOOST_SPIRIT_DEBUG_RULE( abs_path );
            BOOST_SPIRIT_DEBUG_RULE( rel_path );
            BOOST_SPIRIT_DEBUG_RULE( rel_segment );
            BOOST_SPIRIT_DEBUG_RULE( scheme );
            BOOST_SPIRIT_DEBUG_RULE( authority );
            BOOST_SPIRIT_DEBUG_RULE( reg_name );
            BOOST_SPIRIT_DEBUG_RULE( server );
            BOOST_SPIRIT_DEBUG_RULE( userinfo );
            BOOST_SPIRIT_DEBUG_RULE( hostport );
            BOOST_SPIRIT_DEBUG_RULE( host );
            BOOST_SPIRIT_DEBUG_RULE( hostname );
            BOOST_SPIRIT_DEBUG_RULE( domainlabel );
            BOOST_SPIRIT_DEBUG_RULE( toplabel );
            BOOST_SPIRIT_DEBUG_RULE( ipv4_address );
            BOOST_SPIRIT_DEBUG_RULE( port );
            BOOST_SPIRIT_DEBUG_RULE( path_segments );
            BOOST_SPIRIT_DEBUG_RULE( segment );
            BOOST_SPIRIT_DEBUG_RULE( param );
            BOOST_SPIRIT_DEBUG_RULE( pchar );
            BOOST_SPIRIT_DEBUG_RULE( query );
            BOOST_SPIRIT_DEBUG_RULE( fragment );
            BOOST_SPIRIT_DEBUG_RULE( uric );
            BOOST_SPIRIT_DEBUG_RULE( reserved );
            BOOST_SPIRIT_DEBUG_RULE( unreserved );
            BOOST_SPIRIT_DEBUG_RULE( mark );
            BOOST_SPIRIT_DEBUG_RULE( escaped );
            BOOST_SPIRIT_DEBUG_RULE( alnumdash );
            
        }
        
         rule<typename lexeme_scanner<ScannerT>::type>& start() const { return uri_reference; }             };};        

intmain(){    cout << "/////////////////////////////////////////////////////////\n\n";    cout << "\t\tExpression parser...\n\n";    cout << "/////////////////////////////////////////////////////////\n\n";    cout << "Type an expression...or [q or Q] to quit\n\n";    uri_grammar uri;    //  Our parser    string str;    while (getline(cin, str))    {        if (str[0] == 'q' || str[0] == 'Q')            break;        parse_info<> info = parse(str.c_str(), uri);        if (info.full)        {            cout << "-------------------------\n";            cout << "Parsing succeeded\n";            cout << "-------------------------\n";        }        else        {            cout << "-------------------------\n";            cout << "Parsing failed\n";            cout << "stopped at: \": " << info.stop << "\"\n";            cout << "-------------------------\n";        }    }    cout << "Bye... :-) \n\n";    return 0;}