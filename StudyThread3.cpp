// 데드락이 발생하는 상태

#include <iostream>
#include <thread>
#include <mutex>

void Worker1(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10000; ++i)
	{
		std::lock_guard<std::mutex> Lock1(m1);
		std::lock_guard<std::mutex> Lock2(m2);
	}
}

void Worker2(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10000; ++i)
	{
		std::lock_guard<std::mutex> Lock2(m2);
		std::lock_guard<std::mutex> Lock1(m1);
	}
}

int main()
{
	int Counter = 0;
	std::mutex m1;
	std::mutex m2;

	std::thread t1(Worker1, std::ref(m1), std::ref(m2));
	std::thread t2(Worker2, std::ref(m1), std::ref(m2));

	t1.join();
	t2.join();

	std::cout << "끝" << std::endl;
}