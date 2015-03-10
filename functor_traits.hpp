// ***********************************************************************
// Assembly         :
// Author           : Antonio ( Joe ) Colino
// Created          : 01-09-2015
//
// Last Modified By : Joe
// Last Modified On : 01-09-2015
// ***********************************************************************
// <copyright file="functor_traits.h">
//          Copyright Antonio ( Joe ) Colino 2014 - 2015.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// All documentation provide here within.
// </copyright>
// <summary></summary>
// ***********************************************************************
#ifndef FUNCTOR_TRAITS_HEADER_H
#define FUNCTOR_TRAITS_HEADER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif // # pragma once

#include <type_traits>
#include <functional>

namespace joe
{
    template<typename FunctorType>
    /// class is_functor is the only exposed functor trait.
    /// value  : true if the type is a functor object , false otherwise
    /// type   : typedefs the functor as a function ptr, and any other type that is cast able to func ptr...
    /// result : casts a functor object instance to a corresponding std::function object
    class is_functor
    {
    private:
        template<class... Types>
        struct arg_count
        {
            static const int value = sizeof...( Types );
        };
        // all possible logic trains are continued for typedef purposes,
        // although not doing so would certainly condense this
    private:
        template< bool value_type >
        struct functor_tag : public std::false_type { };
        template< >
        struct functor_tag< true > : public std::true_type{ };
        // generic case , continue for typedef
        template < bool value_type , typename T>
        struct functor_traits : public functor_tag < value_type >
        {
            static const int arity = 0;
            typedef void return_type;
            typedef typename T type;
            typedef typename std::function<T> function;
        };
        // false case ( never a non member pointer ) , continue for typedef
        template< bool value_type , typename ReturnType , typename... FArgs>
        struct functor_traits< value_type , ReturnType( *)( FArgs... ) > 
            : public std::false_type{
            static const int arity = arg_count<FArgs...>::value;
            typedef typename ReturnType return_type;
            typedef typename ReturnType( *type )( FArgs... );
            typedef typename std::function<ReturnType( *)( FArgs... )> function;
        };
        // generic case , continue for typedef
        template< bool value_type , typename Class , typename ReturnType , typename... FArgs>
        struct functor_traits< value_type , ReturnType( Class::* )( FArgs... )  > 
            : public functor_tag < value_type >{
            static const int arity = arg_count<FArgs...>::value;
            typedef typename ReturnType return_type;
            typedef ReturnType( *type )( FArgs... );
            typedef typename std::function<ReturnType( FArgs... )> function;
        };
        // generic case const , continue for typedef
        template< bool value_type , typename Class , typename ReturnType , typename... FArgs>
        struct functor_traits< value_type , ReturnType( Class::* )( FArgs... ) const > 
            : public functor_tag < value_type >{
            static const int arity = arg_count<FArgs...>::value;
            typedef typename ReturnType return_type;
            typedef ReturnType( *type )( FArgs... );
            typedef typename std::function<ReturnType( FArgs... )> function;
        };
        // generic case volatile , continue for typedef
        template< bool value_type , typename Class , typename ReturnType , typename... FArgs>
        struct functor_traits< value_type , ReturnType( Class::* )( FArgs... ) volatile >
            : public functor_tag < value_type >{
            static const int arity = arg_count<FArgs...>::value;
            typedef typename ReturnType return_type;
            typedef ReturnType( *type )( FArgs... );
            typedef typename std::function<ReturnType( FArgs... )> function;
        };
        // generic case const volatile , continue for typedef
        template< bool value_type , typename Class , typename ReturnType , typename... FArgs>
        struct functor_traits< value_type , ReturnType( Class::* )( FArgs... ) const volatile >
            : public functor_tag < value_type >{
            static const int arity = arg_count<FArgs...>::value;
            typedef typename ReturnType return_type;
            typedef ReturnType( *type )( FArgs... );
            typedef typename std::function<ReturnType( FArgs... )> function;
        };
        /// Struct has_overloaded_operator attempts an operator check
        template <typename T>
        struct has_overloaded_operator
        {
            template<typename T>
            struct type_check_t
            {
                typedef char failure[8];
                template<typename T> failure& operator()( const T& , const T& );
                static const bool value = ( sizeof( ( *( T* ) ( 0 ) , *( T* ) ( 0 ) ) ) != sizeof( failure ) );
            };
            static const bool value = type_check_t<T>::value;
        };
        // false case defined by operator check , continue for typedef
        template <typename T>
        struct no_functor_operator : public functor_traits < false , T > { };
        // true result of operator check
        template<typename Any>
        struct is_functor_test_t
        {
            // condition : The defined operator
            static const bool defoperator = has_overloaded_operator<Any>::value;
            // false result assignment of operator check
            template<bool cond , typename over>
            struct if_t
            {
                static const int arity = no_functor_operator<over>::arity;
                static const bool value = no_functor_operator<over>::value;
                typedef typename no_functor_operator<over>::type type;
                typedef typename no_functor_operator<over>::function function;
                typedef typename no_functor_operator<over>::return_type return_type;
            };
            // true result assignment of operator check
            template< typename over >
            struct if_t < true , over >
            {
                static const int arity = is_functor_test_t<typename decltype( &over::operator() )>::arity;
                static const bool value = is_functor_test_t<typename decltype( &over::operator() )>::value;
                typedef typename is_functor_test_t<typename decltype( &over::operator() )>::type type;
                typedef typename is_functor_test_t<typename decltype( &over::operator() )>::function function;
                typedef typename is_functor_test_t<typename decltype( &over::operator() )>::return_type return_type;
            };
            // arity ( result of if_t => operator check )
            static const int arity = if_t<defoperator , Any >::arity;
            // value ( result of if_t => operator check )
            static const bool value = if_t<defoperator , Any >::value;
            // typedef type  ( result of if_t => operator check )
            typedef typename if_t<defoperator , Any >::type type;
            // typedef function  ( result of if_t => operator check )
            typedef typename if_t<defoperator , Any >::function function;
            // typedef return_type ( result of if_t => operator check )
            typedef typename if_t<defoperator , Any >::return_type return_type;
        };
        // false case ( non member function after positive operator condition )
        template<typename  ReturnType , typename... FArgs>
        struct is_functor_test_t< ReturnType( *)( FArgs... ) >
            : public functor_traits < false , ReturnType( *)( FArgs... ) > { };
        // true case ( member function after positive operator condition )
        template<typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_test_t< ReturnType( Class::* )( FArgs... ) >
            : public functor_traits < true , ReturnType( Class::* )( FArgs... ) > { };
        // true case ( member function after positive operator condition )
        template<typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_test_t< ReturnType( Class::* )( FArgs... ) const>
            : public functor_traits < true , ReturnType( Class::* )( FArgs... ) const > { };
        // true case ( member function after positive operator condition )
        template<typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_test_t< ReturnType( Class::* )( FArgs... ) volatile>
            : public functor_traits < true , ReturnType( Class::* )( FArgs... ) volatile > { };
        // true case ( member function after positive operator condition )
        template<typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_test_t< ReturnType( Class::* )( FArgs... ) const volatile>
            : public functor_traits < true , ReturnType( Class::* )( FArgs... ) const volatile > { };
        // is functor type defed function ptr before define functor operator check logic
        // false case defined by if_c , continue for typedef
        template< typename ReturnType >
        struct is_functor_t : public functor_traits < false , ReturnType > { };
        // is it a functor class object logic ( begins full is_functor_test_t test logic examining operators )
        /// Struct is_functor_c
        template<typename T >
        struct is_functor_c
        {
            static const int arity = is_functor_test_t<T>::arity;
            static const bool value = is_functor_test_t<T>::value;
            typedef typename is_functor_test_t<T>::type type;
            typedef typename is_functor_test_t<T>::function function;
            typedef typename is_functor_test_t<T>::return_type return_type;
        };
        // false case ( non member function ptr before class check )
        /// Struct is_functor_c
        template< typename ReturnType , typename... FArgs>
        struct is_functor_c< ReturnType( *)( FArgs... ) >
            : public functor_traits < false , ReturnType( *)( FArgs... ) > { };
        // false case ( member function ptr before class check )
        /// Struct is_functor_c
        template< typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_c< ReturnType( Class::* )( FArgs... ) >
            : public functor_traits < false , ReturnType( Class::* )( FArgs... ) > { };
        // false case ( member function ptr before class check )
        /// Struct is_functor_c
        template< typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_c< ReturnType( Class::* )( FArgs... ) const >
            : public functor_traits < false , ReturnType( Class::* )( FArgs... ) const > { };
        // false case ( member function ptr before class check )
        /// Struct is_functor_c
        template< typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_c< ReturnType( Class::* )( FArgs... ) volatile >
            : public functor_traits < false , ReturnType( Class::* )( FArgs... ) volatile > { };
        // false case ( member function ptr before class check )
        /// Struct is_functor_c
        template< typename Class , typename ReturnType , typename... FArgs>
        struct is_functor_c< ReturnType( Class::* )( FArgs... ) const volatile >
            : public functor_traits < false , ReturnType( Class::* )( FArgs... ) const volatile > { };
        // check for class scenario condition and rout accordingly
        static const bool condition = std::is_class<FunctorType>::value;
        // if it is a class rout to is_functor_c else rout through is_functor_t checks
        /// Struct if_c
        template < bool cond , typename CheckType >
        struct if_c
        {
            /// The arity
            static const int arity = is_functor_t<CheckType>::arity;
            /// The value
            static const bool value = is_functor_t<CheckType>::value;
            // typedef type  ( result of if_c => is_functor_t check )
            typedef typename is_functor_t<CheckType>::type type;
            // typedef function  ( result of if_c => is_functor_t check )
            typedef typename is_functor_t<CheckType>::function function;
            // typedef return_type  ( result of if_c => is_functor_t check )
            typedef typename is_functor_t<CheckType>::return_type return_type;
        };
        /// Struct if_c
        template < typename CheckType >
        struct if_c < true , CheckType >
        {
            /// The arity
            static const int arity = is_functor_c<CheckType>::arity;
            /// The value
            static const bool value = is_functor_c<CheckType>::value;
            // typedef type  ( result of if_c => is_functor_t check )
            typedef typename is_functor_c<CheckType>::type type;
            // typedef function  ( result of if_c => is_functor_t check )
            typedef typename is_functor_c<CheckType>::function function;
            // typedef return_type  ( result of if_c => is_functor_t check )
            typedef typename is_functor_c<CheckType>::return_type return_type;
        };
    public:
        // The arity ( conditional: returns number of args in the case of a lambda or functor, 0 otherwise )
        static const int arity = if_c<condition , FunctorType>::arity;
        // The value ( conditional: returns true in the case of a lambda or functor, false otherwise )
        static const bool value = if_c<condition , FunctorType>::value;
        // typedef type ( non conditional: attempts to typedef the functor, function, or function ptr regardless of boolean success )
        typedef typename if_c<condition , FunctorType >::type type;
        // the equivalent ( non conditional: std::function representation of the given functor or lambda )
        typedef typename if_c<condition , FunctorType >::function function;
        // typedef return_type ( result of if_c => is_functor_t check ) returns void otherwise
        typedef typename if_c<condition , FunctorType >::return_type return_type;
        // result ( conditional: result of is_functor type instance casted to std::function of generic type )
        static const inline function result( FunctorType& lambda )
        {
            static_assert( is_functor<FunctorType>::value , "The given type is not a functor or lambda." );
            return static_cast< typename function >( lambda );
        }
    };

    template<typename LambdaType>
    /// alias : is_lambda for actual type : is_functor, fitting as a lambda is abbreviated syntax for a functor
    struct is_lambda : public is_functor < LambdaType > { };

    template <typename FunctorType>
    // converts any given functor or lambda to it's equivalent std::function representation
    // unlike the result member of is_functor, this convenience function does not require that the
    // input actually be a functor or lambda, meaning it will convert some other functions as well
    static const inline typename is_functor<FunctorType>::function functor_to_function( FunctorType& lambda )
    {
        return static_cast< typename is_functor<FunctorType>::function >( lambda );
    }

    template <typename LambdaType>
    // converts any given functor or lambda to it's equivalent std::function representation
    // unlike the result member of is_functor, this convenience function does not require that the
    // input actually be a functor or lambda, meaning it will convert some other functions as well
    static const inline typename is_lambda<LambdaType>::function lambda_to_function( LambdaType& lambda )
    {
        return static_cast< typename is_lambda<LambdaType>::function >( lambda );
    }
}

// Boost Software License - Version 1.0 - August 17th , 2003
//
// Permission is hereby granted , free of charge , to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license( the "Software" ) to use , reproduce , display , distribute ,
// execute , and transmit the Software , and to prepare derivative works of the
// Software , and to permit third - parties to whom the Software is furnished to
// do so , all subject to the following :
//
// The copyright notices in the Software and this entire statement , including
// the above license grant , this restriction and the following disclaimer ,
// must be included in all copies of the Software , in whole or in part , and
// all derivative works of the Software , unless such copies or derivative
// works are solely in the form of machine - executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS" , WITHOUT WARRANTY OF ANY KIND , EXPRESS OR
// IMPLIED , INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY ,
// FITNESS FOR A PARTICULAR PURPOSE , TITLE AND NON - INFRINGEMENT.IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY , WHETHER IN CONTRACT , TORT OR OTHERWISE ,
// ARISING FROM , OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#endif // FUNCTOR_TRAITS_HEADER_H
