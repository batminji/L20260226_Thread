#include <iostream>
#include <thread>
#include <vector>

void Worker(int& Counter)
{
	for (int i = 0; i < 10000; ++i)
	{
		++Counter;
	}
}

int main()
{
	int Counter = 0;

	std::vector<std::thread> Workers;
	for (int i = 0; i < 4; ++i)
	{
		Workers.emplace_back(Worker, std::ref(Counter));
	}

	for (int i = 0; i < 4; ++i)
	{
		Workers[i].join();
	}

	std::cout << "Counter 최종 값 : " << Counter << '\n';
	// 나같은 경우 4만이 잘 출력되었는데, 
	// 이는 컴파일러 최적화 때문이다.
}