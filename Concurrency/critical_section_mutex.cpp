#include <thread>
#include <mutex>

std::mutex mtx;

int val = 0;

void task ()
{
        for (int i=1; i<=10000; ++i)
        {
                mtx.lock();
                val++;
                mtx.unlock();
        }
}

int main ()
{
        std::thread t1(task);
        std::thread t2(task);

        t1.join();
        t2.join();

        std::cout << val << "\n";

        return 0;
}