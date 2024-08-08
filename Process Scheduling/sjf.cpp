#include <iostream>
#include <bits/stdc++.h>

class SJF_NON_PREEMPTIVE
{
protected:
    std::vector<int> m_arrival_time;
    std::vector<int> m_burst_time;
    std::vector<int> m_completetion_time;
    std::vector<int> m_turnaround_time;
    std::vector<int> m_wait_time;

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> scheduler;

    int m_n;
    int m_clock;
    int m_ind;

    int m_total_wait_time;
    
public:
    bool m_executionComplete;

    SJF_NON_PREEMPTIVE(const int n, const std::vector<int> &arrival_time, const std::vector<int> &burst_time)
        : m_n(n)
        , m_arrival_time(arrival_time)
        , m_burst_time(burst_time)
        , m_clock(0)
        , m_ind(0)
        , m_executionComplete(false)

    {
        scheduler.push({m_burst_time[0], 0});
        m_completetion_time.resize(m_n);
        m_turnaround_time.resize(m_n);
        m_wait_time.resize(m_n);
        m_total_wait_time = 0;
    }
    
    virtual void nextProcess ()
    {
        if (m_executionComplete) return;

        int process = scheduler.top().second;

        m_clock = m_clock + m_burst_time[process];

        m_completetion_time[process] = m_clock;
        m_turnaround_time[process] = m_completetion_time[process] - m_arrival_time[process];
        m_wait_time[process] = m_turnaround_time[process] - m_burst_time[process];
        m_total_wait_time += m_wait_time[process];

        std::cout << "Clock : " << m_clock << ", process : "<< process + 1 << std::endl;

        scheduler.pop();
        
        for(m_ind=m_ind+1; m_ind<=m_clock && m_ind<m_n; ++m_ind)
        {
            if (m_arrival_time[m_ind] <= m_clock)
            {
                scheduler.push({m_burst_time[m_ind], m_ind});
            }
        }

        if (scheduler.empty()) 
        {
            m_executionComplete = true;
        }
    }

    void printCompletionTime () const
    {
        if (!m_executionComplete) return;

        std::cout << "Completion time: ";
        for (int i=0; i<m_n; ++i)
        {
            std::cout << m_completetion_time[i] << " ";
        } 
        std::cout << std::endl;
    }

    void printTurnaroundTime () const
    {
        if (!m_executionComplete) return;

        std::cout << "Turnaround time: ";
        for (int i=0; i<m_n; ++i)
        {
            std::cout << m_turnaround_time[i] << " ";
        } 
        std::cout << std::endl;
    }

    void printWaitTime () const 
    {
        if (!m_executionComplete) return;

        std::cout << "Wait       time: ";
        for (int i=0; i<m_n; ++i)
        {
            std::cout << m_wait_time[i] << " ";
        } 
        std::cout << std::endl;        
    }

    double getAvgWaitTime () const
    {
        if (!m_executionComplete) return -1;

        return (double) m_total_wait_time / m_n;
    }
};

class SJF_PREEMPTIVE : public SJF_NON_PREEMPTIVE
{
private:
    std::vector<int> m_current_burst_time;
public:
    SJF_PREEMPTIVE (const int n, const std::vector<int> &arrival_time, const std::vector<int> &burst_time) 
        : SJF_NON_PREEMPTIVE(n, arrival_time, burst_time)
        , m_current_burst_time(m_burst_time)
    {
    }

    void nextProcess () override
    {
        if (m_executionComplete) return;

        int process = scheduler.top().second;
        scheduler.pop();
        
        while (true)
        {
            m_current_burst_time[process] -= 1;
            ++m_clock;

            if (m_current_burst_time[process] == 0)
            {
                m_completetion_time[process] = m_clock;
                m_turnaround_time[process] = m_completetion_time[process] - m_arrival_time[process];
                m_wait_time[process] = m_turnaround_time[process] - m_burst_time[process];
                m_total_wait_time += m_wait_time[process];
                std::cout << "Clock : " << m_clock << ", process : "<< process + 1 << std::endl;
                break;
            }

            if (++m_ind < m_n && m_arrival_time[m_ind] <= m_clock)
            {
                scheduler.push({m_current_burst_time[m_ind], m_ind});
                if (m_current_burst_time[m_ind] < m_current_burst_time[process])
                {
                    scheduler.push({m_current_burst_time[process], process});
                    std::cout << "Clock : " << m_clock << ", process : "<< process + 1 << std::endl;
                    break;
                }
            }

        }

        if (scheduler.empty()) 
        {
            m_executionComplete = true;
        }
    }
};

int main(int argv, char **argc)
{

    std::vector<int> arrival_time = {0, 1, 2, 3};
    std::vector<int> burst_time = {8, 4, 9, 5};

    SJF_PREEMPTIVE sjf(4, arrival_time, burst_time);

    while (!sjf.m_executionComplete)
    {
        sjf.nextProcess();
    }

    sjf.printCompletionTime();
    sjf.printTurnaroundTime();
    sjf.printWaitTime();

    std::cout << "Avg wait   time: " <<  sjf.getAvgWaitTime() << "\n";

    return 0;
}