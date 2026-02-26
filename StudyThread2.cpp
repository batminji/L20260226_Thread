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

	std::cout << "Counter ÃÖÁ¾ °ª : " << Counter << '\n';
}