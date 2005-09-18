/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef   MACHINA_LOGPROB_HPP
#define   MACHINA_LOGPROB_HPP

#include  <cmath>
#include  <climits>
#include  <boost/operators.hpp>

////////////////////////////////////////////////////////////////////////////////
/// 
/// \brief
/// Machina is a library for manipulating state machines and transducers
/// with an emphasis on flexibility, standard C++ interfaces, and speed 
///
////////////////////////////////////////////////////////////////////////////////
namespace machina { namespace logmath {

////////////////////////////////////////////////////////////////////////////////
///
/// \brief 
/// Implements probabilities using logarithms.
///
/// Probabilistic searching techniques usually require a lot of 
/// repeated multiplication of small probabilities.  Representing the
/// probability as a floating point value quickly leads to underflow errors.
/// Representing the probability as the logarithm delays the underflow issue,
/// but can lead to confusing code.
///
/// basic_logprob implements a probability as a logarithm, while allowing 
/// the user to treat it as a floating point value.
///
/// All arithmetic, ordering, and equality operations are allowed between
/// basic_logprob values.
///
/// \code
///     logprob(0.5) + logprob(0.5) == logprob::one;
///     logprob(0.3) * logprob(0.3) == logprob(0.09);  
/// \endcode
///
/// \param Precision
///     Controls the base of the logarithm used.  Setting Precision = N
///     sets the base of the logarithm equal to 1.0 + 1.0e-N.
///
////////////////////////////////////////////////////////////////////////////////
template <unsigned int Precision>
class basic_logprob 
: boost::ordered_field_operators< basic_logprob<Precision> > {
public:
    basic_logprob (double p = 0.0);
    
    ////////////////////////////////////////////////////////////////////////////
    ///
    ///  All field operations between logprobs are allowed.  The only
    ///  caveat is that the operations clamp at 0.0 and 1.0, so order 
    ///  of operations may have to be considered.
    ///
    ///////////////////////////////////////////////////////////////////////////
    ///\{ 
    void operator += (basic_logprob other);
    void operator -= (basic_logprob other);   //not implemented...
    void operator *= (basic_logprob other);
    void operator /= (basic_logprob other);
    ///\}
    
    bool operator == (basic_logprob other) const 
                     {return m_log == other.m_log;}
                     
    bool operator <  (basic_logprob other) const
                     {return m_log < other.m_log;}
                     
    template <unsigned int Prec>            
    friend std::ostream& 
    operator << (std::ostream& out, basic_logprob<Prec> p);
    
private:
    long m_log;
    class utility {
        long log(double p);
        double exp(long l);
        long add(long l1, long l2);
        
        double m_base;
        double m_logbase;
        long m_add_array_size;
        long* m_add_array;
        
        friend class basic_logprob;
        
        template <unsigned int Prec>            
        friend std::ostream& 
        operator << (std::ostream& out, basic_logprob<Prec> p);
        
    public:
        utility();
        ~utility();
    };
    static utility c_util;
    
public:
    /// represents floating point 0.0
    static basic_logprob zero;
    /// represents floating point 1.0
    static basic_logprob one;
};

////////////////////////////////////////////////////////////////////////////////

template<unsigned int Precision>
typename basic_logprob<Precision>::utility  
basic_logprob<Precision>::c_util;

template<unsigned int Precision>
basic_logprob<Precision> 
basic_logprob<Precision>::zero(0.0);

template<unsigned int Precision>
basic_logprob<Precision>  
basic_logprob<Precision>::one(1.0);

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline basic_logprob<Precision>::utility::utility():
m_base(1.0 + ::pow(0.1,double(Precision))),
m_logbase(::log(m_base)),
m_add_array_size(long(::log(1.0/(m_base - 1.0))/double(m_logbase))),
m_add_array(NULL)
{
    m_add_array = new long[m_add_array_size];
    for(int i=0; i < m_add_array_size; ++i) 
        m_add_array[i] = long(::log(1.0 + pow(m_base,i))/m_logbase + 0.5);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline basic_logprob<Precision>::utility::~utility()
{
    delete[] m_add_array;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline long basic_logprob<Precision>::utility::add(long l1, long l2)
{
    long t;
    if (l1 > l2) std::swap(l1,l2);
    t = l2-l1;
    if (t > m_add_array_size) return l2;
    return l1 + m_add_array[t];
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline long basic_logprob<Precision>::utility::log(double d)
{
    return long(::log(d)/(m_logbase) - 0.5);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline double basic_logprob<Precision>::utility::exp(long l)
{
    return ::exp(double(l)*m_logbase);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline void basic_logprob<Precision>::operator *= (basic_logprob other)
{
    m_log += other.m_log;
    if (m_log >= 0) m_log = LONG_MIN;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline void basic_logprob<Precision>::operator /= (basic_logprob other)
{
    m_log -= other.m_log;
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline void basic_logprob<Precision>::operator += (basic_logprob other)
{
    m_log = c_util.add(m_log, other.m_log);
}

////////////////////////////////////////////////////////////////////////////////

template <unsigned int Precision>
inline basic_logprob<Precision>::basic_logprob(double p):
m_log((p > 0.0 && p < 1.0) ? (c_util.log(p)) : (p <= 0.0 ? LONG_MIN : 0)) 
{}

}; //namespace logmath

using logmath::basic_logprob;

////////////////////////////////////////////////////////////////////////////////
///
/// \brief
/// implements the cmu sphinx logarithm probability scale, a proven scale 
/// to begin with.
///
////////////////////////////////////////////////////////////////////////////////

typedef basic_logprob<5> logprob;

////////////////////////////////////////////////////////////////////////////////

}; // namespace machina
      
#endif // MACHINA_LOGPROB_HPP      
      
        
        