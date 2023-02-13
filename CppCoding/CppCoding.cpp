// CppCodeTest.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <iostream>
#include <string>
#include <stdio.h> //strtok
#include <vector>
#include <list>

#include <iomanip>
#include <complex>
#include <cmath>


#include <thread>
#include <future>
#include <algorithm> //count_if?
//#include "CPP11-14-17-20.h"
#include "async.h"

//#1-7-2019+

#include <complex> //5-7-2019

//Exercises in Modern C++
// Collected and Compiled by Twenkid from various sources
//#!? If they are in the headers or source directory: Compiled twice
//Already defined, redefined symbols? Why? #pragma once, #ifndef ... 
//Either if moved to the header Files or Source Files? 
//If removed from "include -- also error, can't find them. Exterm? 
//#18-1-2023
//Overall -- good fix/debug session and up-learning of concepts, templates; review the code again! 2:08

#ifdef TEMPLATES_ETC
#include "templates.h" //#27-12-2022
#include "various.h" //#18-1-2023
#include "move.h" //#18-1-2023
#endif


//#13-1-2023
//#include <execution>
#include <chrono>

#include "Python.h" //#30-1-2023

using namespace std;


/*
Test Mat access ptr:

https://answers.opencv.org/question/38/what-is-the-most-effective-way-to-access-cvmat-elements-in-a-loop/
https://docs.opencv.org/3.1.0/d3/d63/classcv_1_1Mat.html#af72a7cf6705c102f05b5760db5d3b6ed
https://en.cppreference.com/w/cpp/numeric/complex

*/


namespace TestCPP {

	static int numargs = 0; //python -- should it be strictly static?

	//--> in CodeGen: in the master C++ code pre-creae arrays etc. with many
	//with mapping to idents etc. and reuse
	// All static
	// 1. PyObject* funcId 2. PyMethodDef ms[] = { };  
	// 3. PyModuleDef Mod = EmbModule { ... };
	// 4. PyObject* PyInit_emb(void) { return PyModule_Create(&EmbModule);
	/* Return the number of arguments of the application command line */
	static PyObject* emb_numargs(PyObject* self, PyObject* args)
	{
		if (!PyArg_ParseTuple(args, ":numargs")) return NULL;
		return PyLong_FromLong(numargs);
	}

	static PyMethodDef EmbMethods[] = {
		{"numargs", emb_numargs, METH_VARARGS,
		 "Return the number of arguments received by the process."},
		{NULL, NULL, 0, NULL}
	};

	static PyModuleDef EmbModule = {
		PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
		NULL, NULL, NULL, NULL
	};

	static PyObject*
		PyInit_emb(void)
	{
		return PyModule_Create(&EmbModule);
	}



	auto chrono1() -> std::chrono::duration<double, std::milli>
	{
		using namespace std::chrono_literals;
		std::cout << "Hello waiter\n" << std::flush;
		auto start = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(1000ms);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		std::cout << "Waited " << elapsed.count() << " ms\n";
		return elapsed;
	}

	void threadCallback(int x, std::string str)
	{
		std::cout << "Passed Number = " << x << std::endl;
		std::cout << "Passed String = " << str << std::endl;
	}

	void thrA(int xi, const char* c)
	{
		printf_s("A::THREAD, %d", xi);
		double x=xi, y=0.0, x1=0.4, y1=0.2, x2, y2;		
		for (int i = 0; i < 100; i++) {
			x2 = x + x1;
			y2 = y + y1;
			//printf_s("### A::THREAD, %f:%f ###",x2,y2);
			printf_s("%s %d:",c, i);
			if (i % 100 == 0) printf_s("\n");
			x += 1.0;
			y += 0.55;
		}
	}

	void thrB(int xi, const char*c, string s)
	{
		printf_s("B:THREAD");
		std::cout << s << ", " << xi << endl;
		double x=xi, y, x1, y1, x2, y2;
		x = 1.01; y = 1.02; x1 = 1.00124; y1 = 1.0057;
		for (int i = 0; i < 100; i++) {
			x2 = x * x1;
			y2 = y * y1;
			//printf_s("*** B::THREAD, %f:%f ***", x2, y2);
			printf_s("%s %d:",c,i);
			if (i % 100 == 0) printf_s("\n");
			x += 0.02;
			y += 0.34;
		}
	}

	void RunThreads() {
		cout << "\n:::: THREADS ::::\n";
		int x = 10;
		std::string str = "Bagaranakata";
		//std::thread T1(&threadCallback, x, str);
		//std::thread T2(&threadCallback, x, str);
		std::thread T1(&thrA, x, "*");
		std::thread T2(&thrB, x, "#" , str);
		std::thread T3(&thrA, x, "@" );
		std::thread T4(&thrB, x, "=", str);
		T1.join();
		T2.join();
		T3.join();
		T4.join();
	}

	void threadRef(int const & x)
	{
		int & y = const_cast<int &>(x);
		y++;
		std::cout << "Inside Thread x = " << x << std::endl;
	}

	int gugugugumain()
	{
		int x = 9;
		std::cout << "In Main Thread : Before Thread Start x = " << x << std::endl;
		std::thread threadObj(threadRef, std::ref(x));
		//threadObj.join();
		std::cout << "In Main Thread : After Thread Joins x = " << x << std::endl;
		return 0;
	}


	void complex()
	{
		using namespace std::complex_literals;
		std::cout << std::fixed << std::setprecision(1);

		std::complex<double> z1 = 1i * 1i;     // imaginary unit squared
		std::cout << "i * i = " << z1 << '\n';

		std::complex<double> z2 = std::pow(1i, 2); // imaginary unit squared
		std::cout << "pow(i, 2) = " << z2 << '\n';

		double PI = std::acos(-1);
		std::complex<double> z3 = std::exp(1i * PI); // Euler's formula
		std::cout << "exp(i * pi) = " << z3 << '\n';

		std::complex<double> z4 = 1. + 2i, z5 = 1. - 2i; // conjugates
		std::cout << "(1+2i)*(1-2i) = " << z4*z5 << '\n';
	}

	struct EvalItem {
	public:
		float value;
		int range, fga, fia, nCrit;
		EvalItem(float value, int_fast32_t range, int_fast32_t fia, int_fast32_t fga = 0, int_fast32_t nCrit = 3) :
			value(value), range(range), fga(fga), fia(fia), nCrit(nCrit)
		{
		}
		EvalItem() : value(0), range(1), fga(0), fia(0), nCrit(3) {}

	};


	void TestSort() {

		std::list <pair<string, int>> da;

		da.push_back(std::pair<string, int>("two", 3));
		da.push_back(std::pair<string, int>("Five", 5));
		da.push_back(std::pair<string, int>("One", 1));
		da.push_back(std::pair<string, int>("Nine", 9));

		da.sort([](const pair<string, int>&a, const pair<string, int>&b)
		{
			return (a.second > b.second);
		});

		for (auto i : da) {
			cout << i.first << "==" << i.second << endl;
		}

		cout << "EvalItem Sort Lambda?" << endl;
		vector<EvalItem*> EvalArr(10);
		float G = 24.4, Gg = 43.0, Ga = 56.0;
		float AveBlob = 15.0;
		int_fast32_t range = 2;

		EvalItem i0(G, 1, 0, 0, 3);
		EvalItem i1(G - AveBlob, 1, 1, 3);
		EvalItem i2(Gg, range, 0, 0, 2); //0,0?
		EvalItem i3(Ga, range, 1, 0, 2); //1,0?
		//EvalGroup fork(i1, i2, i3, i4);
		//EvalForkArr.push_back(fork);
		EvalItem* items[4];
		items[0] = &i0; items[1] = &i1;
		items[2] = &i2; items[3] = &i3;

		//vector<EvalItem*> EvalItemForkArr;
		list<EvalItem*> EvalItemForkArr;
		//list<EvalItem*> OutArr;

		for (auto cur : items)
		{
			printf("cur->value=%f :: ", cur->value);
			if (EvalItemForkArr.size() > 1)
			{
				//auto cur = EvalItemForkArr.front();
				auto iter = EvalItemForkArr.begin();
				//std::advance(iter, 1);
				while (iter != EvalItemForkArr.end()) {
					auto next = *iter;
					
					if (cur->value > next->value) //DESC
					//if (cur->value < next->value) //ASC
					{
						EvalItemForkArr.insert(iter, cur);						
						break;
					}
					else std::advance(iter, 1);
				}

				if (iter == EvalItemForkArr.end()) EvalItemForkArr.insert(iter, cur);
				//The new item is bigger than the rest, append at the end
			}
			//else EvalItemForkArr.insert(EvalItemForkArr.begin(), cur); //first element
			else EvalItemForkArr.push_back(cur); //first element
		}

		cout << "EvalItemForkArr New sort DESC..." << endl;

		int n = 0;
		for (auto x : EvalItemForkArr) {
		cout << n << ": " << x->value << endl;
		n++;
		}

		cout << "LAMBDA EvalItemForkArr Sort DESC..." << endl;
		//Lambda Sort
		EvalItem* another[4];
		another[0] = &i0; another[1] = &i1;
		another[2] = &i2; another[3] = &i3;

		//vector<EvalItem*> Arr;
		list<EvalItem*> Arr;
		Arr.push_back(&i0);
		Arr.push_back(&i1);
		Arr.push_back(&i2);
		Arr.push_back(&i3);

		Arr.sort([](EvalItem *a, EvalItem *b)
		{
		   return (a->value > b->value);
		});

		n = 0;
		for (auto i : Arr) {
		  cout << n << ": " << i->value << "==" << i->range << endl;
		  n++;
		}		

	}
	
	/*
	int main() //Should be outside the namespace!
	{
		string t;
		//TestSort();
		cin >> t;
		return 0;
	}
	*/

	void ComplexNumbers() {
		float dx, dy, g;
		dx = 24; dy = 12; g = 10;
		std::complex<float> c(dx, dy);
		c /= g;
		auto magnitude = std::abs(c);
		auto phase = std::arg(c);

		float x[20], y[10];

		x[0] = 10.; y[0] = 10.;
		x[1] = 15.; y[1] = 15.;
		dx = x[0] - x[1];
		dy = y[0] - y[1];
		auto a = std::atan2f(dx, dy);
		cout << "atan: " << dx << ", " << dy << " ... = " << a << "   " << a / 3.1415*360. << endl;
		dx = x[0] - 0.0; dy = y[0] - 0.0;
		dx = 1; dy = 0;
		a = std::atan2f(dx, dy);
		cout << "atan: " << dx << ", " << dy << " ... = " << a << "   " << a / 3.1415*360. << endl;

		for (int i = 0; i < 10; i++)
		{
			cout << "dx?";
			cin >> dx;
			cout << "dy?";
			cin >> dy;
			a = std::atan2f(dx, dy);
			cout << "atan: " << dx << ", " << dy << " ... = " << a << "   " << a / 3.1415*360. << endl;
		}
	
	}

}

using namespace TestCPP;

template <typename T>
int CountTwos(const T& container) {
	return std::count_if(std::begin(container), std::end(container), [](int item) {
		return item == 2;
	});
}

void beginend() {
	cout << "beginend" << endl;
	std::vector<int> vec = { 2, 2, 43, 435, 4543, 534 };
	int arr[8] = { 2, 43, 45, 435, 32, 32, 32, 32 };
	auto a = CountTwos(vec); // 2
	auto b = CountTwos(arr);  // 1
	cout << a << endl; //
	cout << b << endl;
}

/*
void templates_call() { //#27-12-2022
	templates::array_1();
	int it1 = 55, it2 = 66;
	auto a = templates::mymax(it1, it2);
	auto b = templates::mymax(0.345, 45.232);
	std::string s{ "kgdf" }, t{ "jdfwsdfwe" };
	auto c = templates::mymax(s, t);
}
*/

void python(int argc, char* argv[]) { //#30-1-2023
	std::cout << "py?";
	string modules_path = "z:\\";
	numargs = argc;
	PyImport_AppendInittab("emb", &PyInit_emb);
	//These two lines initialize the numargs variable, and make the emb.numargs() function accessible to the embedded Python interpreter.With these extensions, the Python script can do things like
	//       
	//Initialize the python instance
	Py_Initialize();
	PyObject* sysPath = PySys_GetObject((char*)"path");
	PyList_Append(sysPath, (PyUnicode_FromString(modules_path.c_str())));

	PyRun_SimpleString("from time import time,ctime\n"
		"print('Today is', ctime(time()))\n"		
		"import emb\n"
		"print('Number of arguments', emb.numargs())\n"
		);


	string path = "C:\\PY\\tk\\tk.py";
	//FILE* f = fopen(path.c_str(), "rt");
	FILE* f;
	auto err = fopen_s(&f, path.c_str(), "rt");
	char* content = new char[10000];
	size_t r = fread_s(content, 9999,1,9999, f);
	content[r] = 0;
	fclose(f);
	//fcloseall(); //deprecated
	//_fcloseall();
	try {
		PyRun_SimpleString(content);
	}
	catch (PyBaseExceptionObject e) {
		cout << "Error in PyRun_SimpleString" << content;
	}

	//string code = "from time import time,ctime\nprint('Today is',ctime(time())')";
	//PyRun_SimpleString(code.c_str());

	//PyRun_SimpleString("from time import time,ctime\n  \
		print('Today is',ctime(time()))\nimport emb\n  \
		print('Number of arguments', emb.numargs())"); 

	//Run a python function
	PyObject* pName, * pModule, * pFunc, * pArgs, * pValue;
	pName = PyUnicode_FromString((char*)"testlib");
	pModule = PyImport_Import(pName);
	pFunc = PyObject_GetAttrString(pModule, (char*)"test"); //func
	pArgs = PyTuple_Pack(1, PyUnicode_FromString((char*)"Greg"));
	pValue = PyObject_CallObject(pFunc, pArgs);
	auto result = _PyUnicode_AsString(pValue);
	std::cout << result << std::endl;

	PyRun_SimpleString("def g1(c):\n  a=c/43; b=a*a; print(b)\n"
		"g1(464);\n"
		"print('Today is',ctime(time()))\n");

}

int main(int argc, char* argv[])
{
	//python(argc, argv);
	std::thread T0(&python, argc, argv); // , x, "*");


	chrono1();
	//return 0;
	//templates::templates_call();
#ifdef TEMPLATES_ETC
	various::run(); //#18-1-2023
	templates::main();
	templates::main_2();
	templates::main_3();
#endif

	//string s = "33,45,46,59,120,15,9";
	//char* s = "33,45,46,59,120,15,9";
	char s[] = "33,45,46,59,120,15,9";
	printf_s("strtok_s=%s",s);
	char* tok;
	char* next; // = "a";
	char del[] = " ,\t\n";
	vector<string> sArr;
	tok = strtok_s(s, del, &next);
	printf_s("\nSTRTOK:\ntok=%s\n", s);

	while (tok != NULL) {
		//sArr.push_back(tok);
		tok = strtok_s(NULL, del, &next);
		printf_s("tok=%s\n", tok);
	}
	for (auto i : sArr) {
		cout << i << endl;
	}

	string t;
	TestCPP::TestSort();
	TestCPP::complex();
	TestCPP::ComplexNumbers();
	cout << "enter something to run threads ";
	cin >> t;
	TestCPP::RunThreads();

	cin >> t;

	//CPP11::run(); //#27-2-2020 why defined twice?
	beginend();

	async_main();
	parallel();

	return 0;
}