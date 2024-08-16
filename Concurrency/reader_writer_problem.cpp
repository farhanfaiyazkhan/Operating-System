#include <iostream>
#include <semaphore>
#include <thread>

std::binary_semaphore m(1);
std::binary_semaphore wrt(1);
int rc = 0;

void reader (int i)
{
        m.acquire();
        rc++;
        if (rc == 1)
        {
                wrt.acquire();
        }
        m.release();

        std::cout << i << " is reading" << "\n";

        m.acquire();
        rc--;
        if (rc == 0)
        {
                wrt.release();
        }
        m.release();
}

void writer (int i)
{
        wrt.acquire();
        std::cout << i << " is writing." << "\n";
        wrt.release();
}


int main (int argv, char** argc)
{
        std::thread threads[10];

        for (int i=0; i<10; ++i)
        {
                if (i < 5)
                {
                        threads[i] = std::thread(reader, i);
                }
                else
                {
                        threads[i] = std::thread(writer, i);
                }
        }

        for (int i=0; i<10; ++i)
        {
                threads[i].join();
        }

        return 0;
}