/*============================================================================
  Copyright 2005 Michael Pust.
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying
  LICENSE file.
  ============================================================================*/


#ifndef   MACHINA_TAGS_HPP
#define   MACHINA_TAGS_HPP

namespace machina {

struct acceptor_tag {};

struct transducer_tag {};

template <typename T> struct is_acceptor_tag {};

template <typename T> struct is_transducer_tag {};

template <> struct is_acceptor_tag<acceptor_tag> {
    enum { value = true }; 
};

template <> struct is_acceptor_tag<transducer_tag> {
    enum { value = false }; 
};

template <> struct is_transducer_tag<acceptor_tag> {
    enum { value = false }; 
};

template <> struct is_transducer_tag<transducer_tag> {
    enum { value = true }; 
};

template <typename SM> struct is_acceptor {
    enum { value = is_acceptor_tag<typename SM::machine_class>::value };
};

template <typename SM> struct is_transducer {
    enum { value = is_transducer_tag<typename SM::machine_class>::value };
};

}; // namespace machina

#endif // MACHINA_TAGS_HPP