/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#include <boost/test/auto_unit_test.hpp>

#include <machina/logprob.hpp>
#include <machina/logmath_io.hpp>

BOOST_AUTO_UNIT_TEST(test_machina_logprob)
{
    using machina::logprob;
    
    BOOST_CHECK_EQUAL(logprob(0) , logprob::zero);
    BOOST_CHECK_EQUAL(logprob(1) , logprob::one);
    
    logprob half(0.5);
    logprob qtr(0.25);
    
    //BOOST_CHECK_PREDICATE(std::less<logprob>(),2,(logprob::zero,half));
    //BOOST_CHECK_PREDICATE(std::less<logprob>(),2,(half,logprob::one));
    
    BOOST_CHECK_PREDICATE(std::less<logprob>(),(logprob::zero)(half));
    BOOST_CHECK_PREDICATE(std::less<logprob>(),(half)(logprob::one));
    
    BOOST_CHECK_EQUAL(half + half , logprob::one);
    BOOST_CHECK_EQUAL(qtr + qtr, half);
}



