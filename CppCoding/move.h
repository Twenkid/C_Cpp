#pragma once  

#include <iostream>

//move.h
//#28-1-2023

namespace MoveTools {
	struct Data {
		Data(size_t s); //Ctr
		Data(const Data&); //Copy Ctr
		Data(Data&&); //Move Ctr

		size_t s;
		int* b;
	};

	const Data getData(size_t s) {
		return Data(s);
	}

	struct DataB {
		DataB() : x_{ 1 } {}
		DataB(int x);
		int x_;		
	};

	void foo(DataB&& x) {
		x.x_ = 42;
	}

	
	void move_func(){
		auto d1 = Data(42);
		auto d2 = std::move(d1);

		auto d3 = getData(42);
		auto d4 = std::move(getData(42)); //X ERROR compiler

		DataB a = 42;
		//DataB& lval_ref_a = &a; //initial value of a reference to non-const should be lval
		//DataB&& rval_ref_a = &a; //rvalue ref (FAIL!)


		DataB&& rval_ref_a = 42; //rvalue ref OK
		DataB&& b = 42;
		//foo(b);  //lvalue - take the address CAN'T
		foo(std::move(b));  //lvalue - take the address OK
		foo(DataB(73));

	}
     template <typename T, typename U>
	 decltype(auto) Add(T t, U u) {
		 return t + u;
	 };

	 /* unexpected token 'identifier', expected ; ? 
	 template <typename T>
	 decltype(auto) Wrapper(T&& t) {
		 //...
		 return std::forward<T> t;
	 };
	 */

	 struct Type {
		 int a;
		 int& ref_a = a;
		 int getA() { return int(73); }
		 int& getRefA() { return ref_a;  }
	 private:
		 Type(int i) : a(int(i)) {} //priv. ctr?
	 };

	int&& f1(int& i) {
		 return std::move(i);
	}


	int move_main() {
		move_func();
		int a = 56734;
		f1(a);
		int i = 73;
		auto a1 = f1(i);
		decltype(auto) b = f1(i); //incomplete type - not allowed? //AAA here
		int&& c = 99;
		decltype(c) d = 55;
		decltype((c)) e = d; //can't be a nconst/number (rvalue)
		//decltype((c)) e = 3235; //can't be a nconst/number (rvalue)
		//Use decltype in order to preserve value category, instead of auto

		//std::declval<T>()  ... incomplete types 
	    DataB t; //needs to have a default ctr
		std::cout << typeid(std::declval<DataB>()).name(); // std::declval<T>()

		//bbut 'b' int differs in levels of indirection from 'int &&' --> & error --> should be with && --> move?
		//&& -- multiple initialization?
		decltype(std::declval<DataB>().x_) &&b1 = 999; //Takes the TYPE of the member variable if it's unknown!
		decltype(std::declval<DataB>().x_) b2 = 333; //Takes the TYPE of the member variable if it's unknown!

		std::declval<Type>().a; //xvalue
		std::declval<Type>().ref_a; //lvalue
		std::declval<Type>().getA(); //prvalue  pure
		std::declval<Type>().getRefA(); //lvalue
		//... forwarding reference ...   36 min ... See: future, more ... 
		// 27 min... Details of binding ... b  
   


		//evaluating std::declval is an error
	}

/*
* * 
* C++ 23 : deducing this ...
*  like_t  <double&, int> //int &
* forward_like<T>(u) ...
* Back to Basics: Master C++ Value Categories With Standard Tools - Inbal Levi - CppCon 2022
* https://www.youtube.com/watch?v=tH0Z2OvHAd8
* 
* https://www.youtube.com/watch?v=tH0Z2OvHAd8&t=877s
* 
* decltype (unlike auto) preserves value category. For an expression of type T:
* 
* if expression is xvalue, yields T&&
* if lvalue, T&
* if prvalue, T
* Instead of a type, as a placeholder, preserves value categories
  Value categories: quality of an expression (not an object)
  C: lvalue, non-lvalue, ...

  C++98: Lvalue, rvalue: non-lvalue
  ...
  C++11: rvalue reference, move semantics
  Peter Dimov ...

  lvalue, xvalue ; prvalue (pure value)

  Can identity, doesn't have, can be moved, can't be moved ...

  C++17 
  C++ guaranteed copy elision ... Instead of temporaries ... 

  prvalue's materialization - even if it's not instantiated ...

  C++20 implicitly move ...
  C++23: deducing this ...
  lifetimes ...sss


  std::decay<T>::type /...
  decltype( expr ) -- the type + value category 

 
*/
	
}
 
