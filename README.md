#lambda_traits on GitHub
The lambda_traits header is a header designed for handling lambdas and functors. Casting to and from std::function, is_lambda, is_functor, function ptr casts, etc. 

Syntax is : 

    bool is_functor_value = joe::is_functor<FuncType>::value;
    bool is_lambda_value = joe::is_lambda<FuncType>::value;
    int arg_count = joe::is_functor<FuncType>::arity;
    int arg_count = joe::is_lambda<FuncType>::arity;
    auto func = joe::functor_to_function( f );
    auto func = joe::lambda_to_function( f );
    auto func = joe::is_lambda<FuncType>::result( f );
    auto func = joe::is_functor<FuncType>::result( f );
    std::enable_if< joe::is_lambda< FuncType >::value >
    std::enable_if< joe::is_functor< FuncType >::value >
    typedef typename joe::is_functor<FuncType>::type signature;
    typedef typename joe::is_functor<FuncType>::return_type return_type;

		
		template< class FunctorType 
			, class = typename std::enable_if<!std::is_function< FunctorType >::value >::type
			, class = typename std::enable_if<!std::is_base_of< ITask , FunctorType >::value >::type
			, class = typename std::enable_if<is_lambda< FunctorType >::value >::type
		>
		// explicit constructor ( functors and lambdas )
		explicit packaged_task( const FunctorType& fnarg )
		: State( new MyStateType( functor_to_function( fnarg ) ) )
		, OriginalFunc( functor_to_function( fnarg ) ) { }
