/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef   MACHINA_URI_GRAMMAR_HPP
#define   MACHINA_URI_GRAMMAR_HPP
#include  <boost/spirit/core.hpp>
#include  <iostream>
#include  <string>

namespace machina 
{

struct uri_grammar : public boost::spirit::grammar<uri_grammar> {
    template <typename ScannerT> struct definition {   
        boost::spirit::rule<ScannerT> 
                       uri_reference, absolute_uri, relative_uri, hier_part,
                       opaque_part, net_path, abs_path, rel_path, rel_segment, 
                       scheme, authority, reg_name, server, userinfo, hostport, 
                       host, hostname, domainlabel, toplabel, ipv4_address, 
                       port, path_segments, segment, param, pchar, query, 
                       fragment, uric, reserved, unreserved, mark, escaped, 
                       uric_no_slash, alnumdash;

        definition(uri_grammar const& self)  
        {
            using boost::spirit;
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///   uri_reference ::= [ absolute_uri | relative_uri ] 
            ///                     [ "#" fragment ]
            ///   absolute_uri  ::= scheme ":" ( hier_part | opaque_part )
            ///   relative_uri  ::= ( net_path | abs_path | rel_path ) 
            ///                     [ "?" query ]
            ///
            ////////////////////////////////////////////////////////////////////
            uri_reference = !( absolute_uri | relative_uri ) >> 
                            !( '#' >> fragment );
                            
            absolute_uri  = scheme >> ':' >> ( hier_part | opaque_part );
                            
            relative_uri  = ( net_path | abs_path | rel_path ) >> 
                            !( '?' >> query );
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   hier_part     = ( net_path | abs_path ) [ "?" query ]         
            ///   opaque_part   = uric_no_slash *uric            
            ///
            ////////////////////////////////////////////////////////////////////
            hier_part   = ( net_path | abs_path ) >> !( '?' >> query );
            opaque_part = uric_no_slash >> *uric;
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   uric_no_slash = unreserved | escaped | ";" | "?" | ":" | "@" |            
            ///                   "&" | "=" | "+" | "$" | ","            
            ///
            ////////////////////////////////////////////////////////////////////
            uric_no_slash = unreserved  | escaped | ';' | '?' | ':' | '@' | 
                            '&' | '=' | '+' | '$' | ',';
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   net_path      = "//" authority [ abs_path ]            
            ///   abs_path      = "/"  path_segments            
            ///   rel_path      = rel_segment [ abs_path ]            
            ///
            ////////////////////////////////////////////////////////////////////
            net_path = "//" >> authority >> !abs_path;
            abs_path = '/' >> path_segments;
            rel_path = rel_segment >> !abs_path;
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   rel_segment   = 1*( unreserved | escaped |            
            ///                       ";" | "@" | "&" | "=" | "+" | "$" | "," )            
            ///
            ////////////////////////////////////////////////////////////////////
            rel_segment = +( unreserved | escaped  | 
                             ';' | '@' | '&' | '=' | '+' | '$' | ',' );
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   scheme        = alpha *( alpha | digit | "+" | "-" | "." )            
            ///
            ////////////////////////////////////////////////////////////////////
            scheme = alpha_p >> *( alpha_p | digit_p | '+' | '-' | '.' );
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   authority     = server | reg_name            
            ///
            ////////////////////////////////////////////////////////////////////
            authority = server | reg_name;
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   reg_name      = 1*( unreserved | escaped | "$" | "," |        
            ///                       ";" | ":" | "@" | "&" | "=" | "+" )       
            ///
            ////////////////////////////////////////////////////////////////////
            reg_name =  +( unreserved | escaped | '$' | ',' |  
                           ';' | ':' | '@' | '&' | '=' | '+' );
            
            ////////////////////////////////////////////////////////////////////
            /// 
            ///   server        = [ [ userinfo "@" ] hostport ] 
            ///   userinfo      = *( unreserved | escaped |
            ///                      ";" | ":" | "&" | "=" | "+" | "$" | "," )
            ///
            ////////////////////////////////////////////////////////////////////
            server   = !( !( userinfo >> '@' ) >> hostport );
            
            userinfo = *( unreserved | escaped | 
                          '$' | ',' | ';' | ':' | '&' | '=' | '+' );
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   hostport      = host [ ":" port ] 
            ///   host          = hostname | ipv4_address
            ///   hostname      = *( domainlabel "." ) toplabel [ "." ]
            ///   domainlabel   = alphanum | alphanum *( alphanum | "-" ) 
            ///                   alphanum            
            ///   toplabel      = alpha | alpha *( alphanum | "-" ) alphanum
            ///   ipv4_address  = 1*digit "." 1*digit "." 1*digit "." 1*digit
            ///   port          = *digit
            ///   alnumdash     = alphanum alnumdash | +dash alphanum alnumdash 
            ///                   | NULL
            ///
            ////////////////////////////////////////////////////////////////////
            hostport     = host >> !( ':' >> port );
            host         = hostname | ipv4_address;
            hostname     = *( domainlabel >> '.' ) >> toplabel >> !ch_p('.');
            domainlabel  = alnum_p >> alnumdash;
            toplabel     = alpha_p >> alnumdash;
            ipv4_address = uint_p >> '.' >> uint_p >> '.' >> 
                           uint_p >> '.' >> uint_p;
            port         = !uint_p;
            alnumdash    = (alnum_p >> alnumdash)               | 
                           (+ch_p('-') >> alnum_p >> alnumdash) | 
                           epsilon_p;
            
            ////////////////////////////////////////////////////////////////////
            ///           
            ///   path          = [ abs_path | opaque_part ]            
            ///   path_segments = segment *( "/" segment )            
            ///   segment       = *pchar *( ";" param )            
            ///   param         = *pchar           
            ///   pchar         = unreserved | escaped |            
            ///                   ":" | "@" | "&" | "=" | "+" | "$" | ","
            ///
            ////////////////////////////////////////////////////////////////////
            
            // why did i comment this out?
            //path          = (!( abs_path | opaque_part ))[printer("path")];            
            path_segments = segment >> *( '/' >> segment );
            segment       = *pchar >> *( ';' >> param );
            param         = *pchar;
            pchar         = unreserved | escaped | 
                            ':' | '@' | '&' | '=' | '+' | '$' | ',';
            
            ////////////////////////////////////////////////////////////////////
            ///            
            ///   query         = *uric
            ///
            ////////////////////////////////////////////////////////////////////
            query = *uric;
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///   fragment      = *uric
            ///
            ////////////////////////////////////////////////////////////////////
            fragment = *uric;
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///   uric          = reserved | unreserved | escaped
            ///   reserved      = ";" | "/" | "?" | ":" | "@" | "&" | "=" |
            ///                   "+" | "$" | ","
            ///   unreserved    = alphanum | mark
            ///   mark          = "-" | "_" | "." | "!" | "~" | "*" | "'" |
            ///                   "(" | ")"
            ///
            ////////////////////////////////////////////////////////////////////
            uric       = reserved | unreserved | escaped;
            
            reserved   = ch_p(';') | '/' | '?' | ':' | '@' | '&' | '=' | 
                         '+' | '$' | ',';              
            unreserved = alnum_p | mark;
            mark       = ch_p('-') | '_' | '.' | '!' | '~' | '*' | '\'' | 
                         '(' | ')';
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///   escaped       = "%" hex hex
            ///
            ////////////////////////////////////////////////////////////////////
            escaped = '%' >> xdigit_p >> xdigit_p;
            
            #ifdef   BOOST_SPIRIT_DEBUG
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
            #endif // BOOST_SPIRIT_DEBUG
            
        }
        
        const boost::spirit::rule<ScannerT>& start() const { 
            return uri_reference; 
        }
    };
}; 

}; // namespace machina

#endif // MACHINA_URI_GRAMMAR_HPP       
