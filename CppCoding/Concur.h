
#include "stdafx.h"
#include <iostream>
#include <algorithm> 
#include <future>
#include <thread>
#include <execution> //(C++17)
#include <coroutine>

using namespace std;

namespace ex = std::execution;

/*
//A Pattern language for expressing concurrency in Cpp - Lucian Radu Teodorescu - CppCon 2022
auto say_hello() {
	return ex::just() //a signal - sender 
		| ex::then([] { // (sender, ftor) --> sender
		std::printf("Hello, concurrent\n");
		return 0;
			});
}

int concurrency() {
	auto [r] = std::this_thread::sync_wait(say_hesssssllo()).value(); 
	return r;
	                             //(sender) --> optional<tuple<vals...>>
	//Sender - describes work, eventually produces a result, similar to a future
}

*/
 
// Pseudo
//task<int> hello_task() {
//	std::printf("Task started\n");
//	co_return 0;
//
//}

//int start_task() {
//	auto [r] = std::this_thread::sync_wait(hello_task()).value());
//	return r;
//}

/*
enum class shape {rock, paper, scissors};

shape player_choose() { return static_cast<shape>(rand() % 3); }
void print_result(shape r1, shape r2) { }

void play_rock_paper_scissors() {
	static_thread_pool pool{ 9 }; //boost
ex:scheduler auto sched = pool.get_scheduler();

	ex::sender auto game_work = ex::when_all(
		ex::schedule(sched) | ex::then(player_choose),
		ex::schedule(sched) | ex::then(player_choose)
    };
  auto [r1, r2] = sync_wait(std::move(game_work)).value();
  print_result(r1, r2);
  }

  //threads are hidden, exoressed concurrency as a graph, no explicit sync, framework ensures efficiency

  //How to describe concurrency?

*/