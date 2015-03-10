#lambda_traits on GitHub
The lambda_traits header is a header designed for handling lambdas and functors. Casting to and from std::function, is_lambda, is_functor, function ptr casts, etc. Syntax is 

template<typename FuncType  >
void try_lambda( FuncType f )
{
    bool is_lambda_value = joe::is_functor<FuncType>::value;
    std::cout << ( ( is_lambda_value ) ? "is lambda" : "is not lambda" ) << std::endl;
    if( is_lambda_value )
    {
        auto func = joe::functor_to_function( f );
        std::cout << "lambda run value : " << ( func( 1.1 , 2.2 , 3.3 ) ) << std::endl;
    }
}

void test_lambda()
{
    double one = 1.12;
    double two = 32.25;
    double three = 3.00;
    auto my_lambda = [ = ]( double a , double b , double c ){ return a + b + c; };
    try_lambda( my_lambda );
    if( std::is_same<joe::is_functor<decltype( my_lambda )>::return_type , double>::value )
    { std::cout << "the return types match" << std::endl; }
    else { std::cout << "the return types do not match" << std::endl; }
    std::cout << "the lambda takes " << joe::is_functor<decltype( my_lambda )>::arity << " arguments" << std::endl;
    PAUSE
}

template< class FunctorType 
			, class = typename std::enable_if<!std::is_function< FunctorType >::value >::type
			, class = typename std::enable_if<!std::is_base_of< ITask , FunctorType >::value >::type
			, class = typename std::enable_if<is_lambda< FunctorType >::value >::type
		>
		// explicit constructor ( functors and lambdas )
		explicit packaged_task( FunctorType&& fnarg )
		: State( new MyStateType( functor_to_function( std::forward<FunctorType>( fnarg ) ) ) )
		, OriginalFunc( functor_to_function( std::forward<FunctorType>( fnarg ) ) ) { }  
		
		
		template< class FunctorType 
			, class = typename std::enable_if<!std::is_function< FunctorType >::value >::type
			, class = typename std::enable_if<!std::is_base_of< ITask , FunctorType >::value >::type
			, class = typename std::enable_if<is_lambda< FunctorType >::value >::type
		>
		// explicit constructor ( functors and lambdas )
		explicit packaged_task( const FunctorType& fnarg )
		: State( new MyStateType( functor_to_function( fnarg ) ) )
		, OriginalFunc( functor_to_function( fnarg ) ) { }
