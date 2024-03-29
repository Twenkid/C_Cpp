#pragma once
#include <array>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <sstream> //boolalpha_test
namespace typestuff{

 void boolalpha_test() {
 {
			// boolalpha output
			std::cout << std::boolalpha
				<< "boolalpha true: " << true << '\n'
				<< "boolalpha false: " << false << '\n';
			std::cout << std::noboolalpha
				<< "noboolalpha true: " << true << '\n'
				<< "noboolalpha false: " << false << '\n';
			// boolalpha parse
			bool b1, b2;
			std::istringstream is("true false");
			is >> std::boolalpha >> b1 >> b2;

			std::cout << '\"' << is.str() << "\" parsed as " << b1 << ' ' << b2 << '\n';

			//noboolalpha: 1, 0
			//boolalpha: true, false
 }

 void tuple_element_test() {
	 // define array and get the type of the element at position 0
	 std::array<int, 10> data{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	 using T = std::tuple_element<0, decltype(data)>::type; // int

	 std::cout << std::boolalpha;
	 std::cout << std::is_same<T, int>::value << '\n';

	 const auto const_data = data;
	 using CT = std::tuple_element<0, decltype(const_data)>::type; // const int
	 // the result of tuple_element depends on the cv-qualification of the tuple-like type
	 std::cout << std::is_same<T, CT>::value << '\n';
	 std::cout << std::is_same<CT, const int>::value << '\n';
	 //Output:

		 //true
		 //false
		 //true
 }

 int typestuff_main()
 {
	 boolalpha_test();
	 tuple_element_test();
 }



}