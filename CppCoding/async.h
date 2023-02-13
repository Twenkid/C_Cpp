#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>
#include <chrono>
//#include <iostream>
//#include <vector>
//#include <numeric>
#include <execution>

//https://en.cppreference.com/w/cpp/thread/async

std::mutex m;
struct X {
	void foo(int i, const std::string& str) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << ' ' << i << '\n';
	}
	void bar(const std::string& str) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << str << '\n';
	}
	int operator()(int i) {
		std::lock_guard<std::mutex> lk(m);
		std::cout << i << '\n';
		return i + 10;
	}
};

template <typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
	auto len = end - beg;
	if (len < 1000)
		return std::accumulate(beg, end, 0);

	RandomIt mid = beg + len / 2;
	auto handle = std::async(std::launch::async,
		parallel_sum<RandomIt>, mid, end);
	int sum = parallel_sum(beg, mid);
	return sum + handle.get();
}


int reduce_main()
{
	std::vector<double> v(10'000'007, 0.5);

	{
		auto t1 = std::chrono::high_resolution_clock::now();
		double result = std::accumulate(v.begin(), v.end(), 0.0);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms = t2 - t1;
		std::cout << std::fixed << "std::accumulate result " << result
			<< " took " << ms.count() << " ms\n";
	}

	{
		auto t1 = std::chrono::high_resolution_clock::now();
		double result = std::reduce(std::execution::par, v.begin(), v.end()); //C++17?
		//double result = sqrt(3434.4563454353);
		//double result = std::accumulate(v.begin(), v.end(), 0.0);
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms = t2 - t1;
		std::cout << "std::reduce result " << result << " took " << ms.count() << " ms\n";
	}
}


int async_main()
{
	std::vector<int> v(10000, 1);
	std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';
	X x;
	// Calls (&x)->foo(42, "Hello") with default policy:
	// may print "Hello 42" concurrently or defer execution
	auto a1 = std::async(&X::foo, &x, 42, "Hello");
	// Calls x.bar("world!") with deferred policy
	// prints "world!" when a2.get() or a2.wait() is called
	auto a2 = std::async(std::launch::deferred, &X::bar, x, "world!");
	// Calls X()(43); with async policy
	// prints "43" concurrently
	auto a3 = std::async(std::launch::async, X(), 43);
	a2.wait();                     // prints "world!"
	std::cout << a3.get() << '\n'; // prints "53"
	return 0;
} // if a1 is not done at this point, destructor of a1 prints "Hello 42" here


void parallel() { //Run all
	async_main();
	reduce_main();

}