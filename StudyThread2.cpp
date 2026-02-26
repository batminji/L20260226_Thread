// 뮤텍스

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

void Worker(int& Counter, std::mutex& m)
{
	for (int i = 0; i < 10000; ++i)
	{
		// lock_guard 객체
		// 뮤텍스를 인자로 받아서 생성하는 객체
		// 이 객체의 생성자에서 뮤텍스를 lock 한다.
		// lock_guard가 소멸될 때 알아서 lock 했던 뮤텍스를 unlock 하게 된다.

		std::lock_guard<std::mutex> lock(m);
		// lock 생성 시에 m.lock()을 실행한다고 보면 됨.
		// 스코프를 빠져 나가면 lock이 소멸되면서 m이 알아서 unlock()됨.

		++Counter; // 임계 영역
		
		// 서로 다른 쓰레드들이 동일한 자원을 사용할 때
		// 발생하는 문제를 경쟁 상태라고 부른다.
		
		// 이를 표현할 때 Counter에 Race Condition이 있다고 표현한다.

		// CPU는 연산을 수행하기 위해서, 
		// CPU의 레지스터라는 곳에서
		// 데이터를 기록한 다음에 연산을 수행한다.

		// 즉, 모든 데이터들은 메모리에 저장되어 있고,
		// 연산할 때마다 메모리에서 레지스터로 값을 가져온 뒤에
		// 빠르게 연산을 하고 다시 메모리에 가져다 놓는 식으로 작동한다.

		// 한 번에 한 쓰레드에서만 뮤텍스의 사용 권한을 가진다.

		// 뮤텍스 lock()과 unlock()사이에 한 쓰레드만이 유일하게
		// 실행할 수 있는 코드 부분을 임계 영역 (critical section)이라고 한다.

		// 뮤텍스가 취득한 쓰레드가 unlock을 하지 않는 상황을
		// 데드락 deadlock이라고 한다.
	}
}

int main()
{
	int Counter = 0;
	std::mutex m;
	// mutex : 상호 배제 (mutual exclusion)
	// 여러 쓰레드들이 동시에 어떠한 코드에 접근하는 것을 배제한다는 의미

	std::vector<std::thread> Workers;
	for (int i = 0; i < 4; ++i)
	{
		Workers.emplace_back(Worker, std::ref(Counter), std::ref(m));
	}

	for (int i = 0; i < 4; ++i)
	{
		Workers[i].join();
	}

	std::cout << "Counter 최종 값 : " << Counter << '\n';
	// 나같은 경우 4만이 잘 출력되었는데, 
	// 이는 컴파일러 최적화 때문이다.
}