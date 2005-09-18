/*============================================================================
  Copyright 2005 Michael Pust.
  
  Distributed under the Boost Software License, Version 1.0.
  See copy at http://www.boost.org/LICENSE_1_0.txt , or the accompanying 
  LICENSE file. 
  ============================================================================*/

#ifndef MACHINA_LOGNUMBER_HPP
#define MACHINA_LOGNUMBER_HPP

#include <cmath>
#include <limits>
#include <boost/operators.hpp>
#include <iostream>

namespace machina { namespace logmath {
    ////////////////////////////////////////////////////////////////////////////
    ///
    /// \brief implements positive numbers using logarithms
    ///
    ////////////////////////////////////////////////////////////////////////////
    template <unsigned int Precision>
    class basic_lognumber : 
    public boost::ordered_field_operators<basic_lognumber<Precision> >
    {
    public:
        explicit basic_lognumber(double number = 0.0);
        
        void operator += (basic_lognumber other);
        void operator -= (basic_lognumber other);
        void operator *= (basic_lognumber other);
        void operator /= (basic_lognumber other);
        
        bool operator <  (basic_lognumber other) const;
        bool operator == (basic_lognumber other) const;
        
        template <unsigned int Prec>            
        friend std::ostream& 
        operator << (std::ostream& out, basic_lognumber<Prec> p);
        
    private:
        double m_number;
        
        class utility
        {
        public:
            double log(double number) const;
            double exp(double number) const;
            
            double add(double number1, double number2) const;
            double subtract(double number1, double number2) const;
            
            utility();
            
        private:  
            double m_base;
            double m_logbase;
            
            friend class basic_lognumber;
            
            template <unsigned int Prec>            
            friend std::ostream& 
            operator << (std::ostream& out, basic_lognumber<Prec> p);
        };
        
        static utility c_util;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    typename basic_lognumber<Precision>::utility 
    basic_lognumber<Precision>::c_util;
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline basic_lognumber<Precision>::basic_lognumber(double number):
    m_number(c_util.log(number))
    {}
    
    ////////////////////////////////////////////////////////////////////////////

    template <unsigned int Precision>
    inline void 
    basic_lognumber<Precision>::operator *= (basic_lognumber other)
    {
        m_number += other.m_number;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline void 
    basic_lognumber<Precision>::operator /= (basic_lognumber other)
    {
        m_number -= other.m_number;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline void 
    basic_lognumber<Precision>::operator += (basic_lognumber other)
    {
        m_number = c_util.add(m_number,other.m_number);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline void 
    basic_lognumber<Precision>::operator -= (basic_lognumber other)
    {
        m_number = c_util.subtract(m_number,other.m_number);
    }

    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline bool
    basic_lognumber<Precision>::operator <  (basic_lognumber other) const
    {
        return m_number < other.m_number;
    }

    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline bool
    basic_lognumber<Precision>::operator == (basic_lognumber other) const
    {
        return m_number == other.m_number;
    }

    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    basic_lognumber<Precision>::utility::utility():
    m_base(1.0 + ::pow(0.1,Precision)),
    m_logbase(::log(m_base))
    {}
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline double
    basic_lognumber<Precision>::utility::exp(double number) const
    {
        return ::exp(m_logbase*number);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline double
    basic_lognumber<Precision>::utility::log(double number) const
    {
        return number > 0.0 ? (::log(number)/m_logbase ):
                              (-std::numeric_limits<double>::infinity());
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline double
    basic_lognumber<Precision>::utility::add(double n1, double n2) const
    {
        return n1 + log(1.0 + ::pow(m_base, n2 - n1));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    template <unsigned int Precision>
    inline double
    basic_lognumber<Precision>::utility::subtract(double n1, double n2) const
    {
        if (n1 <= n2) return -std::numeric_limits<double>::infinity();
        else return n2 + log(::pow(m_base, n1 - n2) - 1.0);
    }
}; // namespace logmath    

    using logmath::basic_lognumber;
    typedef basic_lognumber<5> lognumber;
    
}; //namespace macnina

#endif // MACHINA_LOGNUMBER_HPP