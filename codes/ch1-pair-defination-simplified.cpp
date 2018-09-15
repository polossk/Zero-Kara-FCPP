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
        constexpr pair(const T1& x, const T2& y);
        template<class U, class V> constexpr pair(U&& x, V&& y);
        template<class U, class V> constexpr pair(const pair<U, V>& p);
        template<class U, class V> constexpr pair(pair<U, V>&& p);
        pair& operator=(const pair& p);
        template<class U, class V> pair& operator=(const pair<U, V>& p);
        pair& operator=(pair&& p) noexcept();
        template<class U, class V> pair& operator=(pair<U, V>&& p);
        void swap(pair& p) noexcept();
    };
}
// filename: ch1-pair-defination-simplified.cpp
