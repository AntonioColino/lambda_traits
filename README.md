#lambda_traits on GitHub
The lambda_traits header is a header designed for handling lambdas and functors. Casting to and from std::function, is_lambda, is_functor, function ptr casts, etc. Syntax is 

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