#include <iostream>
#include <bits/stdc++.h>

class SJF_NON_PREEMPTIVE
{
protected:

    int m_n;				// total number of processes
	int m_clock;			// clock
	int m_lap;				// last arrived process
	
	std::vector<int> m_at;	// arrival time
	std::vector<int> m_bt;	// burst time
	std::vector<int> m_ct;	// completion time
	std::vector<int> m_tat; // turnaround time
	std::vector<int> m_wt;	// waiting time

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> scheduler;

    int m_total_wait_time;

    void setCt(const int process, const int clock)
    {
        m_ct[process] = clock;
    }

    void setTat(const int process)
    {
        m_tat[process] = m_ct[process] - m_at[process];
    }

    void setWt(const int process)
    {
        m_wt[process] = m_tat[process] - m_bt[process];
        m_total_wait_time += m_wt[process];
    }

    void updateScheduler()
    {
        while (m_lap < m_n && m_at[m_lap] <= m_clock)
        {
            scheduler.push({m_bt[m_lap], m_lap});
            m_lap++;
        }
    }

public:
    bool m_executionComplete;

    SJF_NON_PREEMPTIVE(const int n, const std::vector<int> &arrival_time, const std::vector<int> &burst_time)
        : m_n(n)
        , m_at(arrival_time)
        , m_bt(burst_time)
        , m_clock(0)
        , m_lap(1)
        , m_executionComplete(false)
        , m_total_wait_time(0)

    {
        scheduler.push({m_bt[0], 0});
        m_ct.resize(m_n);
        m_tat.resize(m_n);
        m_wt.resize(m_n);
    }

    void print(const int clock, const int process) const
    {
        std::cout << "Clock Time : " << clock << ", Process: " << process << "\n";
    }

    virtual void nextProcess()
    {
        if (m_executionComplete) return;

        int process = scheduler.top().second;
        scheduler.pop();

        m_clock += m_bt[process];

        setCt(process, m_clock);
        setTat(process);
        setWt(process);

        print(m_clock, process + 1);

        updateScheduler();

        if (scheduler.empty())
        {
            m_executionComplete = true;
        }
    }

    void printCompletionTime() const
    {
        if (!m_executionComplete) return;

        std::cout << "Completion time: ";
        for (int i = 0; i < m_n; ++i)
        {
            std::cout << m_ct[i] << " ";
        }
        std::cout << std::endl;
    }

    void printTurnaroundTime() const
    {
        if (!m_executionComplete) return;

        std::cout << "Turnaround time: ";
        for (int i = 0; i < m_n; ++i)
        {
            std::cout << m_tat[i] << " ";
        }
        std::cout << std::endl;
    }

    void printWaitTime() const
    {
        if (!m_executionComplete) return;

        std::cout << "Wait       time: ";
        for (int i = 0; i < m_n; ++i)
        {
            std::cout << m_wt[i] << " ";
        }
        std::cout << std::endl;
    }

    double getAvgWaitTime() const
    {
        if (!m_executionComplete) return -1;

        return static_cast<double>(m_total_wait_time) / m_n;
    }
};

class SJF_PREEMPTIVE : public SJF_NON_PREEMPTIVE
{
private:
    std::vector<int> m_rt;

    bool updateScheduler (const int rt) 
    {
        bool preempted = false;
        if (m_lap < m_n && m_at[m_lap] <= m_clock)
        {
            scheduler.push({m_rt[m_lap], m_lap});
            if (m_bt[m_lap] < rt) 
            {
                preempted = true;
            }
            m_lap++;
        }
        return preempted;
    }
public:
    SJF_PREEMPTIVE(const int n, const std::vector<int> &arrival_time, const std::vector<int> &burst_time)
        : SJF_NON_PREEMPTIVE(n, arrival_time, burst_time)
        , m_rt(burst_time)
    {
    }

    void nextProcess() override
    {
        if (m_executionComplete) return;

        int process = scheduler.top().second;
        scheduler.pop();

        while (true)
        {
            m_clock++;
            m_rt[process]--;

            if(updateScheduler(m_rt[process]) || m_rt[process] == 0)
            {
                print(m_clock, process+1);
                
                if (m_rt[process] == 0)
                {
                    setCt(process, m_clock);
                    setTat(process);
                    setWt(process);
                }
                else
                {
                    scheduler.push({m_rt[process], process});
                }

                break;
            }
        }

        if (scheduler.empty())
        {
            m_executionComplete = true;
        }
    }
};

int main(int argc, char **argv)
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

    std::cout << "Avg wait   time: " << sjf.getAvgWaitTime() << "\n";

    return 0;
}
