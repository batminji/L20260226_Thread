#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>

void Worker(std::vector<int>::iterator start, std::vector<int>::iterator end, int* result)
{
	int Sum = 0;
	for (auto itr = start; itr < end; ++itr)
	{
		Sum += *itr;
	}
	*result = Sum;

	std::thread::id ID = std::this_thread::get_id();
	// std::cout << "쓰래드 " << ID << "에서 " << *start << "부터" << *(end - 1) << "까지 계산한 결과 : " << Sum << '\n';
	printf("쓰레드 %x 에서 %d 부터 %d 까지 계산한 결과 : %d \n", ID, *start,
		*(end - 1), Sum);
}

int main()
{
	std::vector<int> Data(10000);
	for (int i = 0; i < 10000; ++i)
	{
		Data[i] = i;
	}

	std::vector<int> PartialSumResult(4);

	std::vector<std::thread> Workers;
	for (int i = 0; i < 4; ++i)
	{
		Workers.push_back(std::thread(Worker, Data.begin() + i * 2500, Data.begin() + (i + 1) * 2500, &PartialSumResult[i]));
	}

	for (int i = 0; i < 4; ++i)
	{
		Workers[i].join();
	}

	int Total = 0;
	for (int i = 0; i < 4; ++i)
	{
		Total += PartialSumResult[i];
	}

	std::cout << "전체 합 : " << Total << '\n';
}