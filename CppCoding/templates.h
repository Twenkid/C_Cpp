//templates.h
#pragma once
#ifndef TEMPLATES_H
//#include "stdafx.h"
#include <iostream>
//#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <set>
#include <vector>
//#include <string>
//#include <stdio.h>

//#27-12-2022
/*
CppCon 2022 Back to basics: Templates 
ssd
* Non-Type template parameter NTTP types

const integral values int, long, enum
std::nullptr_t
Pointers to globally visible obj/func/members
Lvalue reference to obj or functions

NOT supported:
String literals (directly)
Classes

Since C++20 suported:
- float, double, ...
- Data structures with public members
- Lambdas


Functions in a header file: require inline
templates - don't require, because they have to be defined, not just declared.

All operations must be supported

*/
namespace templates {


	void array_0() { //cppreference
		// construction uses aggregate initialization
		std::array<int, 3> a1{ {1, 2, 3} }; // double-braces required in C++11 prior to
											// the CWG 1270 revision (not needed in C++11
											// after the revision and in C++14 and beyond)

		std::array<int, 3> a2 = { 1, 2, 3 };  // double braces never required after =

		std::array<std::string, 2> a3 = { std::string("a"), "b" };

		// container operations are supported
		std::sort(a1.begin(), a1.end());
		std::reverse_copy(a2.begin(), a2.end(),
			std::ostream_iterator<int>(std::cout, " "));

		std::cout << '\n';

		// ranged for loop is supported
		for (const auto& s : a3)
			std::cout << s << ' ';

		// deduction guide for array creation (since C++17)
		[[maybe_unused]] std::array a4{ 3.0, 1.0, 4.0 };  // -> std::array<double, 3>

	}

	void array_1() {		
		std::array<int, 10> a = { { 5,4,3,2,1 } }; //double curly braces!
		//a[1] = 5;
	    std::cout << "\n" << "ARRAY\n\n";

		std::cout << a.size();
		//std::cout << a.size();
		std::cout << a.size() << "elem\n";
		for (size_t i = 0; i < a.size(); ++i) {
			std::cout << a[i] << "\n";
		}
		for (std::array<int, 10>::iterator p = a.begin(); p != a.end(); ++p) {
			std::cout << *p << "\n";
		}

		for (auto p = a.begin(); p != a.end(); ++p) {
			std::cout << *p << "\n";
		}

		for (const auto& elem : a) {
			std::cout << elem << "\n";
		}

		std::cout << "p++?" << std::endl; //It's OK as well

		for (auto p = a.begin(); p != a.end(); p++) {
			std::cout << *p << "\n";
		}

	}

	//USE FOR LOGGING!!!!
	//C++17
   //if constexpr -- compile time if --> while "constexpr" ... Const like with consts are fixed during compile time 
	template<typename T, typename... Types>
	void print(T firstArg, Types... args)
	{
		std::cout << firstArg << '\n'; //first
		if constexpr (sizeof...(args) > 0) print(args...); //call print() for all other args recursively --> but lastly no elements
		//OK if no print() for no arguments declared
	}


	/*
	//Variadic templates
	//ERROR --> not enough params
	template<typename T, typename... Types>
	void print(T firstArg, Types... args)
	{
		std::cout << firstArg << '\n'; //first
		print(args...); //call print() for all other args recursively --> but lastly no elements
	}

	//Must be valid at compile time	
	template<typename T, typename... Types>
	void print(T firstArg, Types... args)
	{
		std::cout << firstArg << '\n'; //first
		if (szeof...(args) > 0) print(args...); //call print() for all other args recursively --> but lastly no elements
		//ERROR, if no print() for no arguments declared
	}
	*/
	//Project --> Properties --> C++ - Language -- standard

	
	//C++20: allows auto ... void add(auto& coll, const auto& val) ...
	//Coll -- collection
	template <typename Coll>
	concept HasPushBack = requires (Coll c, typename Coll::value_type v) //was Coll::value_type v) -- error
	{
		c.push_back(v);
	}; //SEMICOLON!!! ; !!!!

	/*
	template <typename Coll, typename T>
	void add(Coll& coll, const T& val) {
		coll.push_back(val);
	}
 
	template<typename Coll, typename T>	
	void add(Coll& col, const auto& val) {
		coll.push_back(val);
	}
	*/

	void add_concept(HasPushBack auto& coll, const auto& val) {
		coll.push_back(val);
	}


	void add_collection(auto& coll, const auto& val) {
		coll.push_back(val);
	}

	//Different types of collections!
	void add_collection_advanced(auto& coll, const auto& val) {
		if constexpr (requires {coll.push_back(val); }) {
			coll.push_back(val);
		}
		else {
			coll.insert(val);
		}		
	}

	void main() {
		std::cout << "templates.h:main_2: " << '\n';
		std::vector<int> coll1; //push_back
		std::set<int> coll2; //insert 
		add_collection_advanced(coll1, 42); //OK< calls push_back()
		add_collection_advanced(coll2, 42); //OK, calls insert()
	}

	template<typename T>
	T mymax(T a, T b) { return b < a ? a : b; }

	void main_2() {
		std::cout << "templates.h:main_2: " << '\n';
		int i1 = 42, i2 = 77;
		std::cout << mymax(i1, i2); //int, int
		std::cout << mymax(0.7, 33.4); //double, double
		std::string s{ "hi" }, t{"world"};
		std::cout << mymax(s, t); //std::string, std::string

	    s = mymax<std::string>("baba", "arga");

		//e - element, co - collection
		std::vector<int> co(10);
		for (const auto& e : co) {
			std::cout << e << '\n';
		}
		//List, double-linked list, balanced binary tree, hash table - unordered set, map

	}  

	//USE for logging etc.!!!!
	template<typename T>
	void print(const T& co) {
		for (const auto& e : co) {
			std::cout << e << '\n';
		}
	}

	//Abbreviated
	template<typename T>
	void print(const auto& co) {
		for (const auto& e : co) {
			std::cout << e << '\n';
		}
	}

	void main_3() {
		std::cout << "templates.h:main_3: " << '\n';
		std::vector<int> v(5);
		std::set<std::string> s1 { "dfd", "Kakakaka"};
		std::set<std::string> s2{ "2323dsc", "DSFdsfsdf", "aaaaaaadfdfd"};
		print(v);
		print(s1);
		print(s2);

	}


	/*
	
	std::complex<double> c1, c2;
	std:cout << mymax(c1,c2); //ERROR complex  no < supported (operator < LESS-THAN
	std::atomic<int> a1{9}, a2{234}; //ERROR: atomic doesnt' support COPY
	...

	Implicit requirements

	Explicitly define the requirements of the templates

	C++20: CONCEPTS

	Specify the requirements for templates, constraints:
	*/
	template<typename T>
	concept SupportsLessThan = requires(T x) { x < x; };

	template<typename T>
	concept Max = requires(T x) { std::copyable<T>&&SupportsLessThan<T>; };
	
	template<Max T>
	T mymax20(T a, T b) { return b < a ? a : b; };

	//Multiple template params
	template<typename T1, typename T2>
	void print(const T1& v1, const T2& v2) {
		std::cout << v1 << ', ' << v2 << '\n';
	}


	//Better error messages:
	//concept SupportsLessThan not supported
	//concept std::copyable not supported


}
#define TEMPLATES_H
#endif 