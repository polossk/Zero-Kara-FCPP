namespace std{
    template<class T1, class T2>
    struct pair
    {
        using first_type = T1;
        using second_type = T2;
        T1 first;
        T2 second;
        pair(const pair&) = default;
        pair(pair&&) = default;
        constexpr pair();
        /*conditionally-explicit*/ constexpr pair(const T1& x, const T2& y);
        template<class U, class V>
        /*conditionally-explicit*/  constexpr pair(U&& x, V&& y);
        template<class U, class V>
        /*conditionally-explicit*/  constexpr pair(const pair<U, V>& p);
        template<class U, class V>
        /*conditionally-explicit*/  constexpr pair(pair<U, V>&& p);
        template <class... Args1, class... Args2>
        pair(piecewise_construct_t, tuple<Args1...> first_args, tuple<Args2...> second_args);
        pair& operator=(const pair& p);
        template<class U, class V> pair& operator=(const pair<U, V>& p);
        pair& operator=(pair&& p) noexcept(/*see definition*/ );
        template<class U, class V> pair& operator=(pair<U, V>&& p);
        void swap(pair& p) noexcept(/*see definition*/);
    };
}