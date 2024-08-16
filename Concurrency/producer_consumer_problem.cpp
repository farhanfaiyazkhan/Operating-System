#include <iostream>
#include <thread>
#include <semaphore>


std::binary_semaphore m(1);
std::counting_semaphore<5> empty(5);
std::counting_semaphore<5> full(0);

void producer ()
{
        for (int i=0; i<5; ++i)
        {
                empty.acquire();
                m.acquire();

                std::cout << "Producing" << std::endl;

                m.release();
                full.release();
        }
}

void consumer ()
{
        for (int i=0; i<5; ++i)
        {
                full.acquire();
                m.acquire();

                std::cout << "Consuming" << std::endl;

                m.release();
                empty.release();
        }
}

int main (int argv, char **argc)
{
        std::thread p(producer);
        std::thread c(consumer);

        p.join();
        c.join();

        return 0;
}