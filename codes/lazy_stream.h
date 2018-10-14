// ---------------------------------------------------------
// - Class: lazy_stream<>                                  -
// - Immutable stream with non-strict evaluation           -
// - Version: 1.2                                          -
// - Author: David Serrano Martínez. June 2018             -
// ---------------------------------------------------------


// Main references:
//
// http://louisbotterill.blogspot.com.es/2009/07/
//	scala-lazy-evaluation-and-sieve-of.html
//
// http://oldfashionedsoftware.com/2009/07/10/
//	scala-code-review-foldleft-and-foldright/
//


#ifndef LAZY_STREAM_H
#define LAZY_STREAM_H


#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>


template <typename T>
class lazy_stream
{
	typedef T head_type;
	typedef std::shared_ptr<head_type> head_ptr_type;

	typedef lazy_stream<T> tail_type;
	typedef std::shared_ptr<tail_type> tail_ptr_type;

	typedef std::function<lazy_stream<T> ()> tail_gen_type;
	typedef std::shared_ptr<tail_gen_type> tail_gen_ptr_type;

	typedef std::function<bool (const T&)> predicate_fn_type;

	template <typename U>
	friend class lazy_stream;

	head_ptr_type head_ptr_;			// Head pointer
	tail_ptr_type tail_ptr_;			// Tail pointer
	tail_gen_ptr_type tail_gen_ptr_;	// Tail generator pointer
	bool empty_;

	// Assignment operator (private)
	lazy_stream<T>& operator=(const lazy_stream<T>& other);

	// Move assignment operator (private)
	lazy_stream<T>& operator=(lazy_stream<T>&& other);

public:

	// Empty stream
	static lazy_stream<T> nil;

	// Creates the stream: {n, n+1, n+2,...}
	static lazy_stream<T> from(const T& n);

	// Creates the stream: {n, n+step, n+2*step,...}
	static lazy_stream<T> from(const T& n, const T& step);

	// Creates the stream: [from..to)
	static lazy_stream<T> range(const T& from, const T& to);

	// Creates the stream: [from..to) with a step
	static lazy_stream<T> range(const T& from, const T& to, const T& step);

	// Empty stream constructor
	lazy_stream<T>() :
	head_ptr_(nullptr),
	tail_ptr_(nullptr),
	tail_gen_ptr_(nullptr),
	empty_(true) {}

	template <typename U>
	using remove_const_reference=
		typename std::remove_const<typename std::remove_reference<U>::type>;

	// Template constructor for:
	// lazy_stream<T>(head_type&& head, tail_type&& tail),
	// lazy_stream<T>(head_type&& head, const tail_type& tail),
	// lazy_stream<T>(const head_type& head, tail_type&& tail) and
	// lazy_stream<T>(const head_type& head, const tail_type& tail)
	//
	// (Idea borrowed from Lukasz Gwizdz, codeproject member)
	template <
		typename Head_Type,
		typename U,
		typename std::enable_if<
			std::is_same<
				tail_type,
				typename remove_const_reference<U>::type
			>::value,
			int
		>::type=0
	>
	lazy_stream<T>(Head_Type&& head, U&& tail) :
	head_ptr_(std::make_shared<head_type>(std::forward<Head_Type>(head))),
	tail_ptr_(std::make_shared<tail_type>(std::forward<U>(tail))),
	tail_gen_ptr_(nullptr),
	empty_(false) {}

	// Template constructor for:
	// lazy_stream<T>(head_type&& head, tail_gen_type&& tail_gen),
	// lazy_stream<T>(head_type&& head, const tail_gen_type& tail_gen),
	// lazy_stream<T>(const head_type& head, tail_gen_type&& tail_gen) and
	// lazy_stream<T>(const head_type& head, const tail_gen_type& tail_gen)
	//
	// (Idea borrowed from Lukasz Gwizdz, codeproject member)
	template <
		typename Head_Type,
		typename U,
		typename std::enable_if<
			std::is_convertible<	// See the use of is_convertible instead of is_same.
									// A lambda can be converted into a std::function as
									// tail_gen_type, but it is NOT a std::function.
				typename remove_const_reference<U>::type,
				tail_gen_type
			>::value,
			int
		>::type=0
	>
	lazy_stream<T>(Head_Type&& head, U&& tail_gen) :
	head_ptr_(std::make_shared<head_type>(std::forward<Head_Type>(head))),
	tail_ptr_(nullptr),
	tail_gen_ptr_(std::make_shared<tail_gen_type>(std::forward<U>(tail_gen))),
	empty_(false) {}

	// Copy constructor
	lazy_stream<T>(const lazy_stream<T>& other) :
	head_ptr_(other.head_ptr_),
	tail_ptr_(other.tail_ptr_),
	tail_gen_ptr_(other.tail_gen_ptr_),
	empty_(other.empty_) {}

	// Move constructor
	lazy_stream<T>(lazy_stream<T>&& other) :
	head_ptr_(std::move(other.head_ptr_)),
	tail_ptr_(std::move(other.tail_ptr_)),
	tail_gen_ptr_(std::move(other.tail_gen_ptr_)),
	empty_(other.empty_) {}

	// Gets the stream first element (head)
	auto head() const -> const head_type&;

	// Creates the tail substream by removing the head
	lazy_stream<T> tail() const;

	// Is the stream empty?
	bool empty() const {return empty_;}

	// Does the stream contain the value element?
	bool contains(const T& value) const;

	// Gets the number of elements in the stream
	size_t size() const;

	// Does any element comply with exist_fn predicate?
	bool exists(const predicate_fn_type& exist_fn) const;

	// Creates a substream by selecting all the elements which satisfy the filter_fn predicate
	lazy_stream<T> filter(const predicate_fn_type& filter_fn) const;

	// Creates a substream by selecting all the elements which do not satisfy the filter_fn predicate
	lazy_stream<T> filter_not(const predicate_fn_type& filter_fn) const;

	// Gets the element of index n [0..)
	auto get(size_t n) const -> const head_type&;

	// Creates a substream containing the first element satisfying the find_fn predicate
	lazy_stream<T> find_first(const predicate_fn_type& find_fn) const;

	// Creates a substream containing the first element not satisfying the find_fn predicate
	lazy_stream<T> find_first_not(const predicate_fn_type& find_fn) const;

	// Creates the substream of the first n elements
	lazy_stream<T> take(size_t n) const;

	// Creates the substream resulting from dropping the first n elements
	lazy_stream<T> drop(size_t n) const;

	// Creates the substream whose head does not comply with drop_fn predicate
	lazy_stream<T> drop_while(const predicate_fn_type& drop_fn) const;

	// Creates the substream whose head complies with drop_fn predicate
	lazy_stream<T> drop_while_not(const predicate_fn_type& drop_fn) const;

	// Creates the reversed stream
	lazy_stream<T> reverse() const;

	// Pours the stream into a std::list
	std::list<T> to_list() const;

	// Creates a stream by applying the map_fn map
	template <typename U>
	lazy_stream<U> map(const std::function<U (const T&)>& map_fn) const;

	// Fold left reduction
	template <typename U>
	std::function<U (const std::function<U (const U&, const T&)>&)> fold_left(const U& start) const;

	// Left reduction
	template <typename U>
	std::function<U (const std::function<U (const U&, const T&)>&)> reduce_left() const;

	// Creates a stream by pairing corresponding elements in this and other streams
	template <typename U>
	lazy_stream<std::pair<T, U>> zip(const lazy_stream<U>& other) const;

	// Creates a pair of streams by unzipping the pair elements in other
	template <typename U, typename V>
	friend std::pair<lazy_stream<U>, lazy_stream<V>> unzip(const lazy_stream<std::pair<U, V>>& other);

	// Creates a stream by prepending a value element to other stream
	template <typename U>
	friend lazy_stream<U> cons(const U& value, const lazy_stream<U>& other);

	// Creates a stream by prepending a value element to other stream
	template <typename U>
	friend lazy_stream<U> operator&(const U& value, const lazy_stream<U>& other);
};


// Empty stream
template <typename T>
lazy_stream<T> lazy_stream<T>::nil;


// Creates the stream: {n, n+1, n+2,...}
template <typename T>
lazy_stream<T> lazy_stream<T>::from(const T& n)
{
	return lazy_stream<T>(n, [n]() -> lazy_stream<T> {
		return lazy_stream<T>::from(n+T(1), T(1));
		});
}


// Creates the stream: {n, n+step, n+2*step,...}
template <typename T>
lazy_stream<T> lazy_stream<T>::from(const T& n, const T& step)
{
	return lazy_stream<T>(n, [n, step]() -> lazy_stream<T> {
		return lazy_stream<T>::from(n+step, step);
		});
}


// Creates the stream: [from..to)
template <typename T>
lazy_stream<T> lazy_stream<T>::range(const T& from, const T& to)
{
	if(to>from)
		return range(from, to, T(1));

	if(from>to)
		return range(from, to, T(-1));

	return lazy_stream<T>();
}


// Creates the stream: [from..to) with a step
template <typename T>
lazy_stream<T> lazy_stream<T>::range(const T& from, const T& to, const T& step)
{
	if((to>from && step>=T(0)) || (from>to && step<=T(0)))
		return lazy_stream<T>(from, [from, to, step]() -> lazy_stream<T> {
			return lazy_stream<T>::range(from+step, to, step);
			});

	return lazy_stream<T>();
}


// Assignment operator (private)
template <typename T>
lazy_stream<T>& lazy_stream<T>::operator=(const lazy_stream<T>& other)
{
	head_ptr_=other.head_ptr_;
	tail_ptr_=other.tail_ptr_;
	tail_gen_ptr_=other.tail_gen_ptr_;
	empty_=other.empty_;

	return *this;
}


// Move assignment operator (private)
template <typename T>
lazy_stream<T>& lazy_stream<T>::operator=(lazy_stream<T>&& other)
{
	head_ptr_=std::move(other.head_ptr_);
	tail_ptr_=std::move(other.tail_ptr_);
	tail_gen_ptr_=std::move(other.tail_gen_ptr_);
	empty_=other.empty_;

	return *this;
}


// Gets the stream first element (head)
template <typename T>
auto lazy_stream<T>::head() const -> const head_type&
{
	if(empty_)
		throw std::range_error("Class: lazy_stream<>. No head in empty stream.");

	return *head_ptr_;
}


// Creates the tail substream by removing the head
template <typename T>
lazy_stream<T> lazy_stream<T>::tail() const
{
	if(empty_)
		throw std::range_error("Class: lazy_stream<>. No tail in empty stream.");

	if(tail_ptr_)
		return *tail_ptr_;

	return (*tail_gen_ptr_)();
}


// Does the stream contain the value element?
template <typename T>
bool lazy_stream<T>::contains(const T& value) const
{
	if(empty_)
		return false;

	if(value==*head_ptr_)
		return true;

	return tail().contains(value);
}


// Gets the number of elements in the stream
template <typename T>
size_t lazy_stream<T>::size() const
{
	size_t accu=0;
	lazy_stream<T> temp=*this;

	while(!temp.empty_)
	{
		++accu;
		temp=temp.tail();
	}

	return accu;
}


// Does any element comply with exist_fn predicate?
template <typename T>
bool lazy_stream<T>::exists(const predicate_fn_type& exist_fn) const
{
	if(empty_)
		return false;

	if(exist_fn(*head_ptr_))
		return true;

	return tail().exists(exist_fn);
}


// Creates a substream by selecting all the elements which satisfy the filter_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::filter(const predicate_fn_type& filter_fn) const
{
	if(empty_)
		return lazy_stream<T>();

	const tail_type& tail_=tail();

	if(filter_fn(*head_ptr_))
		return lazy_stream<T>(*head_ptr_, [tail_, filter_fn]() -> lazy_stream<T> {
			return tail_.filter(filter_fn);
			});

	return tail_.filter(filter_fn);
}


// Creates a substream by selecting all the elements which do not satisfy the filter_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::filter_not(const predicate_fn_type& filter_fn) const
{
	return filter(std::not1<predicate_fn_type>(filter_fn));
}


// Gets the element of index n [0..)
template <typename T>
auto lazy_stream<T>::get(size_t n) const -> const head_type&
{
	if(n==0)
		return head();

	return tail().get(n-1);
}


// Creates a substream containing the first element satisfying the find_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::find_first(const predicate_fn_type& find_fn) const
{
	if(empty_)
		return lazy_stream<T>();

	if(find_fn(*head_ptr_))
		return lazy_stream<T>(*head_ptr_, lazy_stream<T>());

	return tail().find(find_fn);
}


// Creates a substream containing the first element not satisfying the find_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::find_first_not(const predicate_fn_type& find_fn) const
{
	return find_first(std::not1<predicate_fn_type>(find_fn));
}


// Creates the substream of the first n elements
template <typename T>
lazy_stream<T> lazy_stream<T>::take(size_t n) const
{
	if(empty_ || n<1)
		return lazy_stream<T>();

	const tail_type& tail_=tail();

	return lazy_stream<T>(*head_ptr_, [tail_, n]() -> lazy_stream<T> {return tail_.take(n-1);});
}


// Creates the substream resulting from dropping the first n elements
template <typename T>
lazy_stream<T> lazy_stream<T>::drop(size_t n) const
{
	if(empty_ || n<1)
		return *this;

	return tail().drop(n-1);
}


// Creates the substream whose head does not comply with drop_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::drop_while(const predicate_fn_type& drop_fn) const
{
	if(empty_)
		return lazy_stream<T>();

	if(drop_fn(*head_ptr_))
		return tail().drop_while(drop_fn);

	return *this;
}


// Creates the substream whose head complies with drop_fn predicate
template <typename T>
lazy_stream<T> lazy_stream<T>::drop_while_not(const predicate_fn_type& drop_fn) const
{
	return drop_while(std::not1<predicate_fn_type>(drop_fn));
}


// Creates the reversed stream
template <typename T>
lazy_stream<T> lazy_stream<T>::reverse() const
{
	lazy_stream<T> accu;
	lazy_stream<T> temp=*this;

	while(!temp.empty_)
	{
		accu=cons(*temp.head_ptr_, accu);
		temp=temp.tail();
	}

	return accu;
}


// Pours the stream into a std::list
template <typename T>
std::list<T> lazy_stream<T>::to_list() const
{
	std::list<T> accu;;
	lazy_stream<T> temp=*this;

	while(!temp.empty_)
	{
		accu.push_back(*temp.head_ptr_);
		temp=temp.tail();
	}

	return accu;
}


// Creates a stream by applying the map_fn map
template <typename T>
template <typename U>
lazy_stream<U> lazy_stream<T>::map(const std::function<U (const T&)>& map_fn) const
{
	if(empty_)
		return lazy_stream<U>();

	const tail_type& tail_=tail();

	return lazy_stream<U>(map_fn(*head_ptr_), [tail_, map_fn]() -> lazy_stream<U> {
		return tail_.map(map_fn);
		});
}


// Fold left reduction
//
// Let start be a starting element of type U.
// Let fold_fn be a function: (U, T) => U.
//
// fold_left operates on the n stream elements
// according to the following sequence:
//
// fold_fn(start, elem_0) 	=> u_1;
// fold_fn(u_1, elem_1) 	=> u_2;
// fold_fn(u_2, elem_2) 	=> u_3;
// ...						=> ...
// fold_fn(u_n-2, elem_n-1) => result
//
template <typename T>
template <typename U>
std::function<U (const std::function<U (const U&, const T&)>&)> lazy_stream<T>::fold_left(const U& start) const
{
	lazy_stream<T> temp1=*this;

	return [temp1, start](const std::function<U (const U&, const T&)>& fold_fn) -> U {
		U accu=start;
		lazy_stream<T> temp2=temp1;

		while(!temp2.empty_)
		{
			accu=fold_fn(accu, *temp2.head_ptr_);
			temp2=temp2.tail();
		}

		return accu;
		};
}


// Left reduction
//
// Let reduce_fn be a function: (U, T) => U.
//
// reduce_left operates on the n stream elements
// according to the following sequence:
//
// reduce_fn(elem_0, elem_1) 	=> u_1;
// reduce_fn(u_1, elem_2) 		=> u_2;
// reduce_fn(u_2, elem_3) 		=> u_3;
// ...							=> ...
// reduce_fn(u_n-2, elem_n-1) 	=> result
//
// Observe that T must be convertible to U.
//
template <typename T>
template <typename U>
std::function<U (const std::function<U (const U&, const T&)>&)> lazy_stream<T>::reduce_left() const
{
	if(empty_)
		throw std::range_error("Class: lazy_stream<>. Cannot reduce an empty stream.");

	return tail().fold_left<U>(*head_ptr_);
}


// Creates a stream by pairing corresponding elements in this and other streams
template <typename T>
template <typename U>
lazy_stream<std::pair<T, U>> lazy_stream<T>::zip(const lazy_stream<U>& other) const
{
	if(empty_ || other.empty_)
		return lazy_stream<std::pair<T, U>>();

	const tail_type& tail_=tail();
	const tail_type& other_tail_=other.tail();

	return lazy_stream<std::pair<T, U>>(
				std::pair<T, U>(*head_ptr_, *other.head_ptr_),
				[tail_, other_tail_](){
					return tail_.zip<U>(other_tail_);
					});
}


// Creates a pair of streams by unzipping the pair elements in other
template <typename T, typename U>
std::pair<lazy_stream<T>, lazy_stream<U>> unzip(const lazy_stream<std::pair<T, U>>& other)
{
	if(other.empty())
		return std::pair<lazy_stream<T>, lazy_stream<U>>();

	std::pair<T, U> head=*other.head_ptr_;
	std::pair<lazy_stream<T>, lazy_stream<U>> tail=unzip(other.tail());

	return std::pair<lazy_stream<T>, lazy_stream<U>>(
			cons(head.first, tail.first),
			cons(head.second, tail.second)
			);
}


// Creates a stream by prepending a value element to other stream
template <typename T>
lazy_stream<T> cons(const T& value, const lazy_stream<T>& other)
{
	return lazy_stream<T>(value, other);
}


// Creates a stream by prepending a value element to other stream
template <typename T>
lazy_stream<T> operator&(const T& value, const lazy_stream<T>& other)
{
	return cons(value, other);
}


#endif
