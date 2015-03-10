// lambda_traits_test.cpp : Defines the entry point for the console application.
//

#include "functor_traits.h"
#include <iostream>
#include <string>
#include <vector>

#ifndef PAUSE
#define PAUSE std::cout << "Press the enter key to continue..." << std::endl; std::cin.get();
#endif // !PAUSE


static const inline volatile double dumbAdd( double one , double two , double three )
{
    double answer = one + two + three;
    std::cout << " answer from dumbAdd : " << answer << std::endl;
    return answer;
};


template<typename FuncType  >
void try_lambda( FuncType f )
{
    bool is_lambda_value = joe::is_functor<FuncType>::value;
    std::cout << "the given input " << ( ( is_lambda_value ) ? "is a lambda" : "is not a lambda" ) << std::endl;
    if( is_lambda_value )
    {
        typedef typename joe::is_functor<FuncType>::type signature;
        typedef typename joe::is_functor<FuncType>::return_type return_type;
        //auto func = joe::is_functor<FuncType>::result( f );
        auto func = joe::functor_to_function( f );
        std::cout << "lambda run value : " << ( func( 1.1 , 2.2 , 3.3 ) ) << std::endl;
        std::cout << "the lambda takes " << joe::is_functor<FuncType>::arity << " arguments" << std::endl;
        std::cout << "the signature of the lambda is " << typeid( signature ).name() << std::endl;
        std::cout << "the return type is " << typeid( return_type ).name() << std::endl;
    }
}

void test_lambda()
{
    double one = 1.12;
    double two = 32.25;
    double three = 3.00;
    auto my_lambda = [ = ]( double a , double b , double c ){ return a + b + c; };
    try_lambda( my_lambda );
    PAUSE
}
int main(int argc, char* argv[])
{
    test_lambda();
	return 0;
}

