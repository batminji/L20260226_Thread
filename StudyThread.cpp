#include <iostream>
#include <mutex>
#include <thread>

void Worker1(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10; ++i)
	{
		m1.lock();
		m2.lock();

		std::cout << "Worker1 : " << i << std::endl;

		m2.unlock();
		m1.unlock();
	}
}

void Worker2(std::mutex& m1, std::mutex& m2)
{
	for (int i = 0; i < 10; ++i)
	{
		while (true)
		{
			m2.lock();

			if (!m1.try_lock())
			{
				m2.unlock();
				continue;
			}

			std::cout << "Worker 2 : " << i << std::endl;

			m1.unlock();
			m2.unlock();
			break;
		}
	}
}

int main()
{
	std::mutex m1;
	std::mutex m2;

	std::thread t1(Worker1, std::ref(m1), std::ref(m2));
	std::thread t2(Worker2, std::ref(m1), std::ref(m2));

	t1.join();
	t2.join();

	std::cout << "³¡\n";
}