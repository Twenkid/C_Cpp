//#include "stdafx.h"
#pragma once
#ifndef VARIOUS_H
#include <variant> //
#include <string>
#include <cassert>
#include <iostream>

#include <functional> 
//#include <iostream>
#include <optional>  //


#include <coroutine> //
#include <cstdint>
#include <exception>
//#include <iostream>

#include <thread>
#include <mutex>
#include <chrono>
#include <map>

//#18-1-2023

using namespace std::literals::chrono_literals; //thread, stop_token

namespace various {


    struct sorter_type {
        bool operator()(const std::vector<int>& lhs, const std::vector<int>& rhs) {
            //compare ...
            return true;/*something*/
        }
    } sorter;

    //How C++ 11, 20,... changed the way we write coded  
    //from boost ... optional, variant, C++17-->20 ... paper size from US letter to A5 format of the paper :)
    //C++ 17 - structure binding, custom ... deduction? ..
    void vector_main() {
        std::vector<std::vector<int>> v = { {1,0}, {0,1} };
        for (std::vector<std::vector<int>>::iterator i = v.begin(); i != v.end(); ++i)
            for (std::vector<int>::const_iterator j = i->begin(); j != i->end(); ++j)
            {
                std::cout << j[0] << "\n";
            }

        std::sort(v.begin(), v.end(), sorter);

        for (auto&& vi : v)
            for (auto&& i : vi)
                std::cout << i << "\n";
        std::sort(v.begin(), v.end(),
            [](const auto& lhs, const auto& rhs) {return true; });

    }//vector_mains

    void sort_sorter_type() {
        
        
    }

    std::map<std::string, std::string> g_pages;
    std::mutex g_pages_mutex;

    void save_page(const std::string& url)
    {
        // simulate a long page fetch
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::string result = "fake content";

        std::lock_guard<std::mutex> guard(g_pages_mutex);
        g_pages[url] = result;
    }

    int mutex_main()
    {
        std::thread t1(save_page, "http://foo");
        std::thread t2(save_page, "http://bar");
        t1.join();
        t2.join();

        // safe to access g_pages without lock now, as the threads are joined
        for (const auto& pair : g_pages) {
            std::cout << pair.first << " => " << pair.second << '\n';
        }
    }

    class X {
        std::recursive_mutex m;
        std::string shared;
    public:
        void fun1() {
            std::lock_guard<std::recursive_mutex> lk(m);
            shared = "fun1";
            std::cout << "in fun1, shared variable is now " << shared << '\n';
        }
        void fun2() {
            std::lock_guard<std::recursive_mutex> lk(m);
            shared = "fun2";
            std::cout << "in fun2, shared variable is now " << shared << '\n';
            fun1(); // recursive lock becomes useful here
            std::cout << "back in fun2, shared variable is " << shared << '\n';
        };
    };

    int recursive_mutex_main()
    {
        X x;
        std::thread t1(&X::fun1, &x);
        std::thread t2(&X::fun2, &x);
        t1.join();
        t2.join();
    }


    void f(std::stop_token stop_token, int value)
    {
        while (!stop_token.stop_requested())
        {
            std::cout << value++ << ' ' << std::flush;
            std::this_thread::sleep_for(100ms);
        }
        std::cout << std::endl;
    }

    int stop_token_main()
    {
        std::jthread thread(f, 5); // prints 5 6 7 8... for approximately 3 seconds
        std::this_thread::sleep_for(4s);
        // The destructor of jthread calls request_stop() and join().
    }

    /*

    erase();
    coroutine_1();
    coroutine_2();
    optional_1();
    variant_1();

    */


    void erase() {
        std::cout << "erase:" << '\n';
       
        //cpp20
        std::vector v = { 1,2,3,4,5,6,7,8,9 };
        std::erase(v, [](int i) { return i % 2 == 1; });
        for (int i : v) { std::cout << i << '\n'; }

        std::vector v_old = { 11,12,13,14,15,16 };
        v_old.erase(std::remove_if(v_old.begin(), v_old.end(),
            [](int i) { return i % 2 == 1; }),
            v_old.end());

        for (int i : v_old) { std::cout << i << '\n'; }
       // */
    }


    template <typename T>
    struct Generator
    {
        // The class name 'Generator' is our choice and it is not required for coroutine
        // magic. Compiler recognizes coroutine by the presence of 'co_yield' keyword.
        // You can use name 'MyGenerator' (or any other name) instead as long as you include
        // nested struct promise_type with 'MyGenerator get_return_object()' method.

        struct promise_type;
        using handle_type = std::coroutine_handle<promise_type>;

        struct promise_type // required
        {
            T value_;
            std::exception_ptr exception_;

            Generator get_return_object()
            {
                return Generator(handle_type::from_promise(*this));
            }
            std::suspend_always initial_suspend() { return {}; }
            std::suspend_always final_suspend() noexcept { return {}; }
            void unhandled_exception() { exception_ = std::current_exception(); } // saving
                                                                                  // exception

            template <std::convertible_to<T> From> // C++20 concept
            std::suspend_always yield_value(From&& from)
            {
                value_ = std::forward<From>(from); // caching the result in promise
                return {};
            }
            void return_void() { }
        };

        handle_type h_;

        Generator(handle_type h)
            : h_(h)
        {
        }
        ~Generator() { h_.destroy(); }
        explicit operator bool()
        {
            fill(); // The only way to reliably find out whether or not we finished coroutine,
                    // whether or not there is going to be a next value generated (co_yield)
                    // in coroutine via C++ getter (operator () below) is to execute/resume
                    // coroutine until the next co_yield point (or let it fall off end).
                    // Then we store/cache result in promise to allow getter (operator() below
                    // to grab it without executing coroutine).
            return !h_.done();
        }
        T operator()()
        {
            fill();
            full_ = false; // we are going to move out previously cached
                           // result to make promise empty again
            return std::move(h_.promise().value_);
        }

    private:
        bool full_ = false;

        void fill()
        {
            if (!full_)
            {
                h_();
                if (h_.promise().exception_)
                    std::rethrow_exception(h_.promise().exception_);
                // propagate coroutine exception in called context

                full_ = true;
            }
        }
    };

    Generator<std::uint64_t>
        fibonacci_sequence(unsigned n)
    {
        if (n == 0)
            co_return;

        if (n > 94)
            throw std::runtime_error("Too big Fibonacci sequence. Elements would overflow.");

        co_yield 0;

        if (n == 1)
            co_return;

        co_yield 1;

        if (n == 2)
            co_return;

        std::uint64_t a = 0;
        std::uint64_t b = 1;

        for (unsigned i = 2; i < n; i++)
        {
            std::uint64_t s = a + b;
            co_yield s;
            a = b;
            b = s;
        }
    }

    int coroutine_1()
    {
        try
        {
            auto gen = fibonacci_sequence(10); // max 94 before uint64_t overflows

            for (int j = 0; gen; j++)
                std::cout << "fib(" << j << ")=" << gen() << '\n';
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << '\n';
        }
        catch (...)
        {
            std::cerr << "Unknown exception.\n";
        }
    }


    //coroutine_2
    // 
    //#include <coroutine>
    //#include <iostream>
    //#include <stdexcept>
    //#include <thread>

    auto switch_to_new_thread(std::jthread& out)
    {
        struct awaitable
        {
            std::jthread* p_out;
            bool await_ready() { return false; }
            void await_suspend(std::coroutine_handle<> h)
            {
                std::jthread& out = *p_out;
                if (out.joinable())
                    throw std::runtime_error("Output jthread parameter not empty");
                out = std::jthread([h] { h.resume(); });
                // Potential undefined behavior: accessing potentially destroyed *this
                // std::cout << "New thread ID: " << p_out->get_id() << '\n';
                std::cout << "New thread ID: " << out.get_id() << '\n'; // this is OK
            }
            void await_resume() {}
        };
        return awaitable{ &out };
    }

    struct task
    {
        struct promise_type
        {
            task get_return_object() { return {}; }
            std::suspend_never initial_suspend() { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }
            void return_void() {}
            void unhandled_exception() {}
        };
    };

    task resuming_on_new_thread(std::jthread& out)
    {
        std::cout << "Coroutine started on thread: " << std::this_thread::get_id() << '\n';
        co_await switch_to_new_thread(out);
        // awaiter destroyed here
        std::cout << "Coroutine resumed on thread: " << std::this_thread::get_id() << '\n';
    }

    int coroutine_2()
    {
        std::cout << "various.coroutine_2: " << '\n';
        std::jthread out;
        resuming_on_new_thread(out);
    }

    // optional can be used as the return type of a factory that may fail
    std::optional<std::string> create(bool b) {
        if (b)
            return "Godzilla";
        return {};
    }

    // std::nullopt can be used to create any (empty) std::optional
    auto create2(bool b) {
        return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
    }

    // std::reference_wrapper may be used to return a reference
    auto create_ref(bool b) {
        static std::string value = "Godzilla";
        return b ? std::optional<std::reference_wrapper<std::string>>{value}
        : std::nullopt;
    }

    int optional_1()
    {
        std::cout << "various.optional_1: " << '\n';
        std::cout << "create(false) returned "
            << create(false).value_or("empty") << '\n';

        // optional-returning factory functions are usable as conditions of while and if
        if (auto str = create2(true)) {
            std::cout << "create2(true) returned " << *str << '\n';
        }

        if (auto str = create_ref(true)) {
            // using get() to access the reference_wrapper's value
            std::cout << "create_ref(true) returned " << str->get() << '\n';
            str->get() = "Mothra";
            std::cout << "modifying it changed it to " << str->get() << '\n';
        }
    }

    int variant_1()
    {
        std::cout << "various.variant_1: " << '\n';
        std::variant<int, float> v, w;
        v = 42; // v contains int
        int i = std::get<int>(v);
        assert(42 == i); // succeeds
        w = std::get<int>(v);
        w = std::get<0>(v); // same effect as the previous line
        w = v; // same effect as the previous line

    //  std::get<double>(v); // error: no double in [int, float]
    //  std::get<3>(v);      // error: valid index values are 0 and 1

        try {
            std::get<float>(w); // w contains int, not float: will throw
        }
        catch (const std::bad_variant_access& ex) {
            std::cout << ex.what() << '\n';
        }

        using namespace std::literals;

        std::variant<std::string> x("abc");
        // converting constructors work when unambiguous
        x = "def"; // converting assignment also works when unambiguous

        std::variant<std::string, void const*> y("abc");
        // casts to void const * when passed a char const *
        assert(std::holds_alternative<void const*>(y)); // succeeds
        y = "xyz"s;
        assert(std::holds_alternative<std::string>(y)); // succeeds
    }

    int run() {
        std::cout << "various.run (coroutines, optional, variant): " << '\n';
        recursive_mutex_main();
        stop_token_main();
        mutex_main();
        erase();
        coroutine_1();
        coroutine_2();
        optional_1();
        variant_1();
    }

}
#define VARIOUS_H
#endif