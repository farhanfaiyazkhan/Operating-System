#include <iostream>
#include <bits/stdc++.h>

class RoundRobin
{
private:
	int m_n;				// total number of processes
	int m_tq;				// time quantum
	int m_clock;			// clock
	int m_lap;				// last arrived process

	std::vector<int> m_at;	// arrival time
	std::vector<int> m_bt;	// burst time
	std::vector<int> m_rt;	// remaining time
	std::vector<int> m_ct;	// completion time
	std::vector<int> m_tat; // turnaround time
	std::vector<int> m_wt;	// waiting time

	std::queue<int> m_rq;   // ready queue

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
	}

	bool m_isComplete;

public:

	bool getExecutionStatus() const 
	{
		return m_isComplete;
	}

	RoundRobin(int n, int tq, const std::vector<int>& at, const std::vector<int>& bt)
		: m_n   (n)
		, m_tq  (tq)
		, m_at  (at)
		, m_bt  (bt)
		, m_rt  (bt)
	{
		m_tat.resize(m_n);
		m_ct.resize(m_n);
		m_bt.resize(m_n);
		m_wt.resize(m_n);
		
		m_isComplete = false;
		m_lap = 1;
		m_clock = 0;

		m_rq.push(0);
	}

	void print(const int clock, const int process) const
	{
		std::cout << "Clock Time : " << clock << ", Process: " << process << "\n";
	}

	void nextCycle()
	{
		if (m_isComplete || m_rq.empty()) return;

		int process = m_rq.front();
		m_rq.pop();

		m_clock += std::min (m_tq, m_rt[process]);
		m_rt[process] -= std::min (m_tq, m_rt[process]);

		while (m_lap < m_n && m_at[m_lap] <= m_clock)
		{
			m_rq.push(m_lap);
			m_lap++;
		}
		
		print(m_clock, process + 1);

		if (m_rt[process] == 0)
		{
			setCt(process, m_clock);
			setTat(process);
			setWt(process);
		}
		else
		{
			m_rq.push(process);
		}

		if (m_rq.empty())
		{
			m_isComplete = true;
		}
	}

	void printCompletionTime() const
	{
		if (!m_isComplete) return;

		std::cout << "Completion time: ";
		for (int i = 0; i < m_n; ++i)
		{
			std::cout << m_ct[i] << " ";
		}
		std::cout << std::endl;
	}

	void printTurnaroundTime() const
	{
		if (!m_isComplete) return;

		std::cout << "Turnaround time: ";
		for (int i = 0; i < m_n; ++i)
		{
			std::cout << m_tat[i] << " ";
		}
		std::cout << std::endl;
	}

	void printWaitTime() const
	{
		if (!m_isComplete) return;

		std::cout << "Wait       time: ";
		for (int i = 0; i < m_n; ++i)
		{
			std::cout << m_wt[i] << " ";
		}
		std::cout << std::endl;
	}
};

int main(int argv, char** argc)
{
	std::vector<int> at = { 0, 1, 2, 3, 4, 6 };
	std::vector<int> bt = { 4, 5, 2, 1, 6, 3 };

	RoundRobin rr(6, 2, at, bt);

	while (!rr.getExecutionStatus())
	{
		rr.nextCycle();
	}

	rr.printCompletionTime();
	rr.printTurnaroundTime();
	rr.printWaitTime();

	return 0;
}