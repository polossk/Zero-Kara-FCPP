// ---------------------------------------------------------
// - File: sample                                          -
// - Usage example of lazy_stream<> class                  -
// - Version: 1.2                                          -
// - Author: David Serrano Martínez. June 2018             -
// ---------------------------------------------------------


#include <algorithm>
#include <iostream>
#include <list>
#include <stdexcept>
#include <utility>
#include "lazy_stream.h"


int main(int argn, char *argc[])
{
	try
	{
		//
		// Infinite streams...
		//

		std::cout << "Infinite streams..." << std::endl;

		// Creates the stream: {3, 4, 5,...}.
		lazy_stream<int> stream_a=lazy_stream<int>::from(3);

		// Creates the stream: {4, 8, 12,...} by filtering
		// stream_a. Only multiples of 4 are kept.
		lazy_stream<int> stream_b=stream_a.filter(
			[](int value){
				return value%4==0;
				});

		// Each element x in stream_b is mapped to (x, x+1).
		// The resulting stream is:
		// {(4, 5), (8, 9), (12, 13),...}.
		typedef std::pair<int, int> pair_type;

		lazy_stream<pair_type> stream_c=stream_b.map<pair_type>(
			[](int value){
				return std::pair<int, int>(value, value+1);
				});

		// Pours the first 10 pairs into a std::list.
		std::list<pair_type> pair_list=stream_c.take(10).to_list();

		// Prints the list.
		std::for_each(
			pair_list.begin(),
			pair_list.end(),
			[](const pair_type& pair){
				std::cout << "(" << pair.first << " , " << pair.second << ")" << std::endl;
				});

		// Prints the 100th element of stream_c.
		pair_type pair=stream_c.get(100);
		std::cout << "(" << pair.first << " , " << pair.second << ")" << std::endl;

		// Other operations.
		std::cout << stream_a.head() << std::endl;			// Prints the first element in stream_a.
		std::cout << stream_a.tail().head() << std::endl;	// Prints the second element in stream_a.
		// ...


		// Sieve of Eratosthenes.
		std::cout << "\nSieve of Eratosthenes..." << std::endl;

		// The sieve function object does the hard work.
		std::function<lazy_stream<int> (const lazy_stream<int>&)> sieve=
		[&sieve](const lazy_stream<int>& start){

			int head=start.head();

			lazy_stream<int> temp=start.filter([head](int value){return value%head>0;});

			// Returns a new lazy stream with its own head and tail generator.
			return lazy_stream<int>(head, [&sieve, temp](){return sieve(temp);});
		};

		// prime_numbers holds all the prime numbers.
		lazy_stream<int> prime_numbers=sieve(lazy_stream<int>::from(2));

		// Takes the first 100 prime numbers and pours them into a std::list.
		std::list<int> prime_number_list=prime_numbers.take(100).to_list();

		// Prints the list.
		std::for_each(
			prime_number_list.begin(),
			prime_number_list.end(),
			[](int value){
				std::cout << value << std::endl;
				});


		//
		// Finite streams...
		//

		std::cout << "\nFinite streams..." << std::endl;

		// Creates a lazy stream of 4 integers.
		lazy_stream<int> stream_d=2 & (1 & (4 & (6 & lazy_stream<int>::nil)));

		// Creates a lazy stream of 4 integers: {2, 1, 4, 6}.
		// Keeps only the even numbers: {2, 4, 6}.
		// Prepends 25: {25, 2, 4, 6}.
		// Maps each element x to (x, x+2): {(25, 27), (2, 4), (4, 6), (6, 8)}.
		// Pours the resulting stream into a std::list.
		// And all in a row. What a rigmarole!!!
		std::list<pair_type> rigmarole_list=
		(25 & (2 & (1 & (4 & (6 & lazy_stream<int>::nil)))).filter(
			[](int value){
				return value%2==0;
				})).map<pair_type>(
					[](int value){
						return std::pair<int, int>(value, value+2);
						}).to_list();

		// Prints the list.
		std::for_each(
			rigmarole_list.begin(),
			rigmarole_list.end(),
			[](const pair_type& pair){
				std::cout << "(" << pair.first << " , " << pair.second << ")" << std::endl;
				});

		// Other operations.

		// Creates a stream by discarding all the elements in stream_d
		// until the first value greater or equal to 4 is found.
		// The resulting stream is: {4, 6}.
		lazy_stream<int> stream_e=stream_d.drop_while(
			[](int value){
				return value<4;
				});

		// Adds and prints 0.5 plus all the elements of stream_d.
		std::cout << stream_d.fold_left<double>(0.5)
						([](double accu, int element){
							return accu+element;
							}) << std::endl;	// Prints 13.5.

		// Reverses the range [100, 1).
		lazy_stream<int> stream_f=lazy_stream<int>::range(100, 1).reverse();

		// Zip and unzip.
		std::cout << "\nZip and unzip..." << std::endl;

		// Zips stream_d with itself.
		// The resulting stream is: {(2, 2), (1, 1), (4, 4), (6, 6)}.
		lazy_stream<std::pair<int, int>> stream_g=stream_d.zip(stream_d);

		// Pours stream_g into a std::list.
		std::list<std::pair<int, int>> zipped_list=stream_g.to_list();

		// Prints the list.
		std::for_each(
			zipped_list.begin(),
			zipped_list.end(),
			[](const std::pair<int, int>& pair){
				std::cout << "(" << pair.first << " , " << pair.second << ")" << std::endl;
				});

		// Unzips stream_g.
		// The resulting pair is: ({2, 1, 4, 6}, {2, 1, 4, 6}).
		std::pair<lazy_stream<int>, lazy_stream<int>> stream_h=unzip(stream_g);

		// Pours the first element of the pair into a std::list and prints it.
		std::list<int> unzipped_first_list=stream_h.first.to_list();

		std::for_each(
			unzipped_first_list.begin(),
			unzipped_first_list.end(),
			[](int value){
				std::cout << value << std::endl;
				});

		// Pours the second element of the pair into a std::list and prints it.
		std::list<int> unzipped_second_list=stream_h.second.to_list();

		std::for_each(
			unzipped_second_list.begin(),
			unzipped_second_list.end(),
			[](int value){
				std::cout << value << std::endl;
				});

		// Multiplies all the elements of a stream.
		lazy_stream<int> stream_i=(25 & (2 & (1 & (4 & (6 & lazy_stream<int>::nil)))));

		double product=stream_i.reduce_left<double>()(
							[](double accum, int element){
								return accum * element;
								});

		std::cout << "product: " << product << std::endl;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	std::cout << "\nPress ENTER to finish..." << std::endl;
	std::cin.get();

	return 0;
}
