// 스레드

#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>

// 쓰레드는 Return 값이란 것이 없다.
// 어떤 결과를 반환하고 싶다면 포인터의 형태로 전달하면 됨.
void Worker(std::vector<int>::iterator start, std::vector<int>::iterator end, int* result)
{
	int Sum = 0;
	for (auto itr = start; itr < end; ++itr)
	{
		Sum += *itr;
	}
	*result = Sum;

	// 각 쓰레드에는 고유 아이디 번호가 할당된다! 2828 5730 4a3c 45a8
	std::thread::id ID = std::this_thread::get_id();

	// std::cout과 다르게 printf는 "..." 안에 있는 문자열을 출력할 때
	// 컨텍스트 스위치가 되더라도 다른 쓰레드들이 그 사이에 메시지를 집어 넣을 수 없다.
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
		// thread 생성자의 첫 번째 인자로 함수를 전달하고,
		// 이어서 해당 함수에 전달할 인자들을 나열한다.
		std::thread Temp(Worker, Data.begin() + i * 2500, Data.begin() + (i + 1) * 2500, &PartialSumResult[i]);
		Workers.push_back(std::move(Temp));
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