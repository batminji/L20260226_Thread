#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

void Producer(std::queue<std::string>& DownloadedPages, std::mutex& m, int Index)
{
	for (int i = 0; i < 5; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100 * Index));
		std::string content = "웹 사이트 : " + std::to_string(i) + "from thread(" + std::to_string(Index) + ")\n";

		m.lock();
		DownloadedPages.push(content);
		m.unlock();
	}
}

void Consumer(std::queue<std::string>& DownloadedPages, std::mutex& m, int& NumProcessed)
{
	while (NumProcessed < 25)
	{
		m.lock();
		
		if (DownloadedPages.empty())
		{
			m.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		std::string content = DownloadedPages.front();
		DownloadedPages.pop();

		++NumProcessed;

		m.unlock();

		std::cout << content;
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

int main()
{
	std::queue<std::string> DownloadedPages;
	std::mutex m;

	std::vector<std::thread> Producers;
	for (int i = 0; i < 5; ++i)
	{
		Producers.push_back(std::thread(Producer, std::ref(DownloadedPages), std::ref(m), i + 1));
	}

	int NumProcessed = 0;
	std::vector<std::thread> Consumers;
	for (int i = 0; i < 3; ++i)
	{
		Consumers.push_back(std::thread(Consumer, std::ref(DownloadedPages), std::ref(m), std::ref(NumProcessed)));
	}

	for (int i = 0; i < 5; ++i)
	{
		Producers[i].join();
	}
	for (int i = 0; i < 3; ++i)
	{
		Consumers[i].join();
	}
}