#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

void Producer(std::queue<std::string>& DownloadedPages, std::mutex& m, int Index, std::condition_variable& cv)
{
	for (int i = 0; i < 5; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * Index));
		std::string content = "웹 사이트 : " + std::to_string(i) + "from thread(" + std::to_string(Index) + ")\n";

		m.lock();
		DownloadedPages.push(content);
		m.unlock();

		cv.notify_one();
	}
}

void Consumer(std::queue<std::string>& DownloadedPages, std::mutex& m, int& NumProcessed, std::condition_variable& cv)
{
	while (NumProcessed < 25)
	{
		// unque_lock은 lock_guard와 거의 동일하다.
		// lock_guard의 경우 생성자 말고는 lock을 할 수 없다.
		// unique_lock의 경우 unlock 후에 다시 lock을 할 수 있다.
		// 더불어, wait 함수는 인자로 unique_lock을 인자로 받는다.
		std::unique_lock<std::mutex> lk(m);

		// wait 함수
		// 어떤 조건이 참이 될 때까지 기다릴지 조건을 인자로 전달한다.
		// 해당 조건이 false를 return 한다면, lk를 unlock 한 뒤에 영원히 sleep한다.
		// 해당 조건이 true라면 cv.wait는 그대로 return해서 Consumer의 content를 처리하는 부분이 그대로 실행된다.
		cv.wait(lk, [&] {return !DownloadedPages.empty() || NumProcessed == 25; });
		
		if (NumProcessed == 25)
		{
			lk.unlock();
			return; // 왜 return인지
		}

		std::string content = DownloadedPages.front();
		DownloadedPages.pop();

		++NumProcessed;

		lk.unlock();

		std::cout << content;
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

int main()
{
	std::queue<std::string> DownloadedPages;
	std::mutex m;
	std::condition_variable cv;

	std::vector<std::thread> Producers;
	for (int i = 0; i < 5; ++i)
	{
		Producers.push_back(std::thread(Producer, std::ref(DownloadedPages), std::ref(m), i + 1, std::ref(cv)));
	}

	int NumProcessed = 0;
	std::vector<std::thread> Consumers;
	for (int i = 0; i < 3; ++i)
	{
		Consumers.push_back(std::thread(Consumer, std::ref(DownloadedPages), std::ref(m), std::ref(NumProcessed), std::ref(cv)));
	}

	for (int i = 0; i < 5; ++i)
	{
		Producers[i].join();
	}

	cv.notify_all();

	for (int i = 0; i < 3; ++i)
	{
		Consumers[i].join();
	}
}