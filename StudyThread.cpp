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
				// try_lock 함수는 lock 할 수 있다면 true를,
				// lock 할 수 없다면 false를 리턴합니다.
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

	// 1. 중첩된 lock을 사용하는 것을 피해라.
	// 2. lock을 소유하고 있을 때 유저 코드를 호출하는 것을 피해라.
	// 3. lock들은 언제나 정해진 순서로 획득해라.

	t1.join();
	t2.join();

	std::cout << "끝\n";
}