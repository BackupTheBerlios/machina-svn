/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef   MACHINA_SRGS_GRAMMAR_HPP
#define   MACNINA_SRGS_GRAMMAR_HPP

#include  <boost/spirit/core.hpp>
#include  <boost/spirit/utility/confix.hpp>
#include  <machina/uri_grammar.hpp>

namespace machina
{

struct srgs_grammar : public boost::spirit::grammar<srgs_grammar>
{
    template <typename ScannerT>
    struct definition
    {
        uri_grammar uri;
        
        typedef typename boost::spirit::lexeme_scanner<ScannerT>::type
                lex_scanner_type;
                        boost::spirit::rule< lex_scanner_type >   
                       name_char,
                       constrained_name,
                       abnf_uri_with_media_type,
                       abnf_uri,
                       name,
                       language_code;
                       
                       
        boost::spirit::rule<ScannerT> 
                       gram,
                       language_attachment,
                       rule_name,
                       single_quoted_characters,
                       double_quoted_characters,
                       quoted_characters,
                       weight,
                       repeat,
                       probability,
                       external_rule_ref,
                       token,
                       tag,
                       tag_format,
                       lexicon_uri,
                       self_ident_header,
                       version_number,
                       char_encoding,
                       nm_token,
                       
                       base_uri,
                       
                       declaration,
                       base_decl,
                       language_decl,
                       mode_decl,
                       root_rule_decl,
                       tag_format_decl,
                       lexicon_decl,
                       meta_decl,
                       tag_decl,
                       rule_definition,
                       scope,
                       rule_expansion,
                       rule_alternative,
                       sequence_element,
                       subexpansion,
                       rule_ref,
                       local_rule_ref,
                       special_rule_ref,
                       repeat_operator;

        definition(srgs_grammar const& self)
        {
            using namespace boost::spirit;
            
            abnf_uri                 = lexeme_d['<' >> uri >> '>' ];
            abnf_uri_with_media_type = lexeme_d[abnf_uri >> '~' >> abnf_uri];
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  name_char ::= letter | digit | '.' | '-' | '_' | ':' 
            ///  nm_token  ::= name_char+            ///  name      ::= (letter | '_' | ':') (name_char)*
            ///
            ////////////////////////////////////////////////////////////////////
            name_char = lexeme_d[alpha_p | digit_p | 
                                 '.' | '-' | '_' | ':'];
            
            nm_token  = lexeme_d[+name_char];
            
            name      = lexeme_d[ ( alpha_p | ch_p('_') | ch_p(':') ) 
                                  >> *name_char ];
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  self_ident_header  ::=            ///    '#ABNF' #x20 version_number (#x20 char_encoding)? ';'            ///       [Additional constraints:            ///          - The semicolon (';') must immediately be followed            ///            by an end-of-line.            ///       ]
            /// 
            ////////////////////////////////////////////////////////////////////
            self_ident_header = str_p("#ABNF") >> version_number >>
                                 !(char_encoding ) >> ';';
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  version_number ::= '1.0'
            ///
            ////////////////////////////////////////////////////////////////////                   
            version_number = str_p("1.0");
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  char_encoding ::= nm_token
            ///
            ////////////////////////////////////////////////////////////////////
            char_encoding = nm_token;
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  base_uri ::= abnf_uri
            ///
            ////////////////////////////////////////////////////////////////////
            base_uri = lexeme_d[abnf_uri];
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  rule_name ::= '$' constrained_name
            ///
            ////////////////////////////////////////////////////////////////////            rule_name = lexeme_d[ch_p('$') >> constrained_name];
            
            ////////////////////////////////////////////////////////////////////            ///
            ///  constrained_name  ::= name - (char* ('.' | ':' | '-') char*)
            ///
            ////////////////////////////////////////////////////////////////////                       
            constrained_name = name - 
                               ( 
                                 *(anychar_p - (ch_p('.') | ':' | '-')) >> 
                                 (ch_p('.') | ':' | '-') >> 
                                 *anychar_p
                               );
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  tag_format ::= abnf_uri            ///
            ////////////////////////////////////////////////////////////////////
            tag_format = lexeme_d[abnf_uri];
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  LexiconURI ::= ABNF_URI | ABNF_URI_with_Media_Type            /// 
            ////////////////////////////////////////////////////////////////////
            lexicon_uri = lexeme_d[abnf_uri | abnf_uri_with_media_type];
            
            ////////////////////////////////////////////////////////////////////            ///
            ///  single_quoted_characters ::= ''' [^']* '''            ///    
            ////////////////////////////////////////////////////////////////////
            single_quoted_characters = lexeme_d[
                                         ch_p('\'') >> *(~ch_p('\'')) 
                                                    >> ch_p('\'')
                                       ];
            
            ////////////////////////////////////////////////////////////////////                ///  
            ///  double_quoted_characters ::= '"' [^"]* '"'
            ///
            ////////////////////////////////////////////////////////////////////
            double_quoted_characters = lexeme_d[
                                         ch_p('"') >> *(~ch_p('"')) 
                                                   >> ch_p('"') //"
                                       ];               
            ////////////////////////////////////////////////////////////////////
            ///            ///  quoted_characters ::= single_quoted_characters | 
            ///                        double_quoted_characters            ///
            ////////////////////////////////////////////////////////////////////
            quoted_characters = single_quoted_characters | 
                                double_quoted_characters;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  weight ::= '/' number '/'            ///
            ////////////////////////////////////////////////////////////////////
            weight = lexeme_d['/' >> ureal_p >> '/'];
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  repeat ::= [0-9]+ ('-' [0-9]*)?            ///        [Additional constraints:            ///          - A number to the right of the hyphen must not be            ///            greater than the number to the left of the hyphen.            ///        ]            ///            ////////////////////////////////////////////////////////////////////
            repeat = lexeme_d[uint_p >> !(ch_p('-') >> !uint_p)];
            
            ////////////////////////////////////////////////////////////////////            ///  
            ///  probability ::=    '/' number '/'            ///        [Additional constraints:            ///          - The float value must be in the range of "0.0"            ///            to "1.0" (inclusive).            ///        ]            ///
            ////////////////////////////////////////////////////////////////////
            probability = lexeme_d[
                            '/' >> limit_d(0.0,1.0)[ureal_p] >> '/'
                          ];
            
            ////////////////////////////////////////////////////////////////////            ///
            ///  external_rule_ref ::= '$' abnf_uri | 
            ///                        '$' abnf_uri_with_media_type            ///        [Additional constraints:            ///          - The referenced grammar must have the same mode            ///            ("voice" or "dtmf") as the referencing grammar.            ///          - If the URI reference contains a fragment            ///            identifier, the referenced rule must be a            ///            public rule of another grammar.            ///          - If the URI reference does not contain a fragment            ///            identifier, i.e. if it is an implicit root rule             ///            reference, then the referenced grammar must declare             ///            a root rule.            ///        ]            ///
            ////////////////////////////////////////////////////////////////////
            external_rule_ref = lexeme_d[
                                  '$' >> (abnf_uri | abnf_uri_with_media_type)
                                ];
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  token ::= nm_token | double_quoted_characters
            ///
            ////////////////////////////////////////////////////////////////////
            token =  nm_token | double_quoted_characters ;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  language_attachment ::= '!' language_code
            ///
            ////////////////////////////////////////////////////////////////////
            language_attachment = lexeme_d[ '!' >> language_code ];
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  language_code ::= nm_token            ///          [Additional constraints:            ///            - The language code must be a valid language 
            ///              identifier.            ///          ]
            ///
            ////////////////////////////////////////////////////////////////////
            language_code = lexeme_d[
                              range_p('a','z') >> range_p('a','z') >> 
                              ch_p('-') >> range_p('A','Z') >>
                              range_p('A','Z')
                            ];
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  tag ::=  '{' [^}]* '}'            ///         | '{!{' (Char* - (Char* '}!}' Char*)) '}!}'            ///
            ////////////////////////////////////////////////////////////////////
            tag = lexeme_d[
                    confix_p("{!{",*anych_p,"}!}") | 
                    confix_p("{", *anychar_p, "}")
                  ];
            
            ////////////////////////////////////////////////////////////////////      
            /// +---------------------------------------------------------------
            /// |
            /// |  Syntax Grammar
            /// |
            /// +---------------------------------------------------------------
            ////////////////////////////////////////////////////////////////////
            
            
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  grammar ::= self_ident_header declaration* rule_definition*
            ///
            ////////////////////////////////////////////////////////////////////            gram = self_ident_header  >> *declaration >> *rule_definition;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  declaration ::= baseDecl | languageDecl | modeDecl | 
            ///                  rootRuleDecl | tagFormatDecl |             ///                  lexiconDecl | metaDecl | tagDecl
            ///
            ////////////////////////////////////////////////////////////////////
            declaration = base_decl | language_decl | mode_decl                           | root_rule_decl | tag_format_decl | 
                          lexicon_decl | meta_decl | tag_decl;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  base_decl ::= 'base' base_uri ';'            ///          [Additional constraints:            ///            - A base declaration must not appear more than            ///              once in grammar.            ///          ]
            ///
            ////////////////////////////////////////////////////////////////////
            base_decl = "base" >> base_uri >> ';';
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  language_decl ::= 'language' language_code ';'            ///          [Additional constraints:
            ///            - A language declaration must not appear more than            ///              once in grammar.            ///            - A language declaration is required if the            ///              grammar mode is "voice".            ///          ]
            ///
            ////////////////////////////////////////////////////////////////////
            language_decl = str_p("language") >> lexeme_d[language_code] >> ';';
        
            ////////////////////////////////////////////////////////////////////
            ///            ///  mode_decl ::= 'mode' 'voice' ';' | 'mode' 'dtmf' ';'            ///          [Additional constraints:            ///            - A mode declaration must not appear more than            ///              once in grammar.            ///          ]            ///
            ////////////////////////////////////////////////////////////////////
            mode_decl = str_p("mode") >> ( str_p("voice") | str_p("dtmf") ) >> 
                        ch_p(';');
           
            ////////////////////////////////////////////////////////////////////
            ///            ///  root_rule_decl ::= 'root' rule_name ';'            ///          [Additional constraints:            ///            - A root rule declaration must not appear more            ///              than once in grammar.            ///            - The root rule must be a rule that is defined            ///              within the grammar.            ///          ]
            ///
            ////////////////////////////////////////////////////////////////////
            root_rule_decl = "root" >> rule_name >> ';';
          
            ////////////////////////////////////////////////////////////////////
            ///            ///  tag_format_decl ::= 'tag-format' tag_format ';'            ///          [Additional constraints:            ///            - A tag-format declaration must not appear more            ///              than once in grammar.            ///          ]
            ///
            ////////////////////////////////////////////////////////////////////
            tag_format_decl = "tag-format" >> tag_format >> ';';
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  lexicon_decl ::= 'lexicon' lexicon_uri ';'            ///
            ////////////////////////////////////////////////////////////////////
            lexicon_decl = "lexicon" >> lexicon_uri >> ';';
          
            ////////////////////////////////////////////////////////////////////
            ///            ///  meta_decl ::= 'http-equiv' quoted_characters 'is' 
            ///                  quoted_characters ';'            ///                | 'meta' quoted_characters 'is' 
            ///                  quoted_characters ';'            ///
            ////////////////////////////////////////////////////////////////////
            meta_decl = ( str_p("http-equiv") | str_p("meta") ) >>
                        quoted_characters >> "is" >> quoted_characters >> ';';
                       
            ////////////////////////////////////////////////////////////////////
            ///            ///  tag_decl ::= tag ';'            ///
            ////////////////////////////////////////////////////////////////////
            tag_decl = tag >> ';';
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  rule_definition ::= scope? rule_name '=' rule_expansion ';'            ///          [Additional constraints:            ///            - The rule name must be unique within a grammar,            ///              i.e. no rule must be defined more than once            ///              within a grammar.            ///          ]            ///
            ////////////////////////////////////////////////////////////////////
            rule_definition = (!scope) >> rule_name >> '=' >> 
                              rule_expansion >> ';';
                              
            ////////////////////////////////////////////////////////////////////
            ///            ///  scope ::= 'private' | 'public'
            ///
            ////////////////////////////////////////////////////////////////////
            scope = str_p("private") | str_p("public");
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  rule_expansion ::= rule_alternative ( '|' rule_alternative )*            ///
            ////////////////////////////////////////////////////////////////////
            rule_expansion = rule_alternative % '|' ;
            
            ////////////////////////////////////////////////////////////////////
            ///
            ///  rule_alternative ::= weight? sequence_element+
            ///
            ////////////////////////////////////////////////////////////////////
            rule_alternative = !weight >> +sequence_element;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  sequence_element ::= subexpansion | 
            ///                       subexpansion repeat_operator
            ///
            ////////////////////////////////////////////////////////////////////
            sequence_element = subexpansion >> !repeat_operator;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  subexpansion ::= token language_attachment?            ///                   | rule_ref             ///                   | tag            ///                   | '(' ')'            ///                   | '(' rule_expansion ')' language_attachment?            ///                   | '[' rule_expansion ']' language_attachment?            ///
            ////////////////////////////////////////////////////////////////////
            subexpansion = ( token >> !language_attachment )                   | rule_ref                    | tag                   | ( ch_p('(') >> ch_p(')') )                   | ( '(' >> rule_expansion >> ')' >> !language_attachment )
                   | ( '[' >> rule_expansion >> ']' >> !language_attachment );
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  rule_ref ::= local_rule_ref | external_rule_ref | 
            ///               special_rule_ref            ///
            ////////////////////////////////////////////////////////////////////
            rule_ref = local_rule_ref | external_rule_ref | special_rule_ref;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  local_rule_ref ::= rule_name            ///          [Additional constraints:            ///            - The referenced rule must be defined within the            ///              same grammar.            ///          ]
            ///            ////////////////////////////////////////////////////////////////////
            local_rule_ref = rule_name;
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  special_rule_ref ::= '$NULL' | '$VOID' | '$GARBAGE'            ///
            ////////////////////////////////////////////////////////////////////
            special_rule_ref = str_p("$NULL") | 
                               str_p("$VOID") | 
                               str_p("$GARBAGE");
            
            ////////////////////////////////////////////////////////////////////
            ///            ///  repeat_operator  ::= '<' repeat probability? '>'                ///            ////////////////////////////////////////////////////////////////////           repeat_operator = '<' >> repeat >> !probability >> '>';
            
            #ifdef   BOOST_SPIRIT_DEBUG
            BOOST_SPIRIT_DEBUG_RULE( name_char );
            BOOST_SPIRIT_DEBUG_RULE( constrained_name );
            BOOST_SPIRIT_DEBUG_RULE( abnf_uri_with_media_type );
            BOOST_SPIRIT_DEBUG_RULE( abnf_uri );
            BOOST_SPIRIT_DEBUG_RULE( name );
            BOOST_SPIRIT_DEBUG_RULE( language_code );            
            BOOST_SPIRIT_DEBUG_RULE( gram );
            BOOST_SPIRIT_DEBUG_RULE( language_attachment );
            BOOST_SPIRIT_DEBUG_RULE( rule_name );
            BOOST_SPIRIT_DEBUG_RULE( single_quoted_characters );
            BOOST_SPIRIT_DEBUG_RULE( double_quoted_characters );
            BOOST_SPIRIT_DEBUG_RULE( quoted_characters );
            BOOST_SPIRIT_DEBUG_RULE( weight );
            BOOST_SPIRIT_DEBUG_RULE( repeat );
            BOOST_SPIRIT_DEBUG_RULE( probability );
            BOOST_SPIRIT_DEBUG_RULE( external_rule_ref );
            BOOST_SPIRIT_DEBUG_RULE( token );
            BOOST_SPIRIT_DEBUG_RULE( tag );
            BOOST_SPIRIT_DEBUG_RULE( tag_format );
            BOOST_SPIRIT_DEBUG_RULE( lexicon_uri );
            BOOST_SPIRIT_DEBUG_RULE( self_ident_header );
            BOOST_SPIRIT_DEBUG_RULE( version_number );
            BOOST_SPIRIT_DEBUG_RULE( char_encoding );
            BOOST_SPIRIT_DEBUG_RULE( nm_token );
            BOOST_SPIRIT_DEBUG_RULE( base_uri );
            BOOST_SPIRIT_DEBUG_RULE( declaration );
            BOOST_SPIRIT_DEBUG_RULE( base_decl );
            BOOST_SPIRIT_DEBUG_RULE( language_decl );
            BOOST_SPIRIT_DEBUG_RULE( mode_decl );
            BOOST_SPIRIT_DEBUG_RULE( root_rule_decl );
            BOOST_SPIRIT_DEBUG_RULE( tag_format_decl );
            BOOST_SPIRIT_DEBUG_RULE( lexicon_decl );
            BOOST_SPIRIT_DEBUG_RULE( meta_decl );
            BOOST_SPIRIT_DEBUG_RULE( tag_decl );
            BOOST_SPIRIT_DEBUG_RULE( rule_definition );
            BOOST_SPIRIT_DEBUG_RULE( scope );
            BOOST_SPIRIT_DEBUG_RULE( rule_expansion );
            BOOST_SPIRIT_DEBUG_RULE( rule_alternative );
            BOOST_SPIRIT_DEBUG_RULE( sequence_element );
            BOOST_SPIRIT_DEBUG_RULE( subexpansion );
            BOOST_SPIRIT_DEBUG_RULE( rule_ref );
            BOOST_SPIRIT_DEBUG_RULE( local_rule_ref );
            BOOST_SPIRIT_DEBUG_RULE( special_rule_ref );
            BOOST_SPIRIT_DEBUG_RULE( repeat_operator );   
            #endif // BOOST_SPIRIT_DEBUG
        }
        
        const boost::spirit::rule<ScannerT>& start() const { return gram; } 
        


    };
};

}; // namespace machina

#endif // MACHINA_SRGS_GRAMMAR_HPP
