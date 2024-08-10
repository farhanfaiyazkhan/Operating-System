#include <iostream>
#include <thread>
#include <atomic>

bool flag[2] = {false, false};
int turn;
std::atomic<int> val(0);

void taskA ()
{
        for (int i=0; i<10000; ++i)
        {
                flag[0] = true;
                turn = 1;
                while (flag[1] && turn == 1);
                val++;
                flag[0] = false;
        }
}

void taskB ()
{
        for (int i=0; i<10000; ++i)
        {
                flag[1] = true;
                turn = 0;
                while (flag[0] && turn == 0);
                val++;
                flag[1] = false;
        }
}

int main ()
{
        std::thread t1(taskA);
        std::thread t2(taskB);

        t1.join();
        t2.join();

        std::cout << val.load() << "\n";

        return 0;
}