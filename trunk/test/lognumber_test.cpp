/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#include <boost/test/auto_unit_test.hpp>
#include "../lognumber.hpp"
#include "../logmath_io.hpp"

#define MACHINA_LOGNUMBER_CHECK_CLOSE(A,B,C)                   \
BOOST_CHECK_PREDICATE(std::less<lognumber>(),((A) - (B)) (C)); \
BOOST_CHECK_PREDICATE(std::less<lognumber>(),((B) - (A)) (C))

BOOST_AUTO_UNIT_TEST(test_machina_lognumber)
{
    using machina::lognumber;
    
    lognumber half(0.5);
    lognumber qtr(0.25);
    lognumber zero(0.0);
    lognumber one(1.0);
    lognumber two(2.0);
    lognumber three(3.0);
    lognumber four(4.0);
    lognumber nine(9.0);
    lognumber eleven(11.0);
    lognumber eighteen(18.0);
    lognumber eightyone(81);
    
    lognumber tolerance(0.001);
    
    /// first, try a little addition...
    MACHINA_LOGNUMBER_CHECK_CLOSE(half + half, one,      tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(nine + nine, eighteen, tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(nine + two,  eleven,   tolerance);
    
    /// next, try a little multiplication...
    MACHINA_LOGNUMBER_CHECK_CLOSE(half * half, qtr,       tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(nine * nine, eightyone, tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(nine * two,  eighteen,  tolerance);
    
    /// some subtraction...
    MACHINA_LOGNUMBER_CHECK_CLOSE(half - half,   zero,      tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(half - qtr,    qtr,       tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(one - qtr,     three*qtr, tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(eleven - nine, two,       tolerance);
    
    /// and division...
    MACHINA_LOGNUMBER_CHECK_CLOSE(three / three,        one,        tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(half / two,           qtr,        tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(eightyone / eighteen, nine / two, tolerance);
    MACHINA_LOGNUMBER_CHECK_CLOSE(two / qtr,            four * two, tolerance);
    
    //BOOST_CHECK_PREDICATE(std::less<lognumber>(),2,(zero,half));
    //BOOST_CHECK_PREDICATE(std::less<lognumber>(),2,(half,one));
    
    BOOST_CHECK_PREDICATE(std::less<lognumber>(),(zero)(half));
    BOOST_CHECK_PREDICATE(std::less<lognumber>(),(half)(one));
    
    /// make sure we can tell the difference between numbers....
    BOOST_CHECK_EQUAL(three != two,            true);
    BOOST_CHECK_EQUAL(three != lognumber(3.0), false);
    BOOST_CHECK_EQUAL(four == nine,            false);
    BOOST_CHECK_EQUAL(four == lognumber(4.0),  true);
    
    /// and make sure order relations are in order.
    BOOST_CHECK_EQUAL( three < four, true);
    BOOST_CHECK_EQUAL(three >= four, false);
    BOOST_CHECK_EQUAL(half >= qtr,   true);
    BOOST_CHECK_EQUAL(half < qtr,    false);
}