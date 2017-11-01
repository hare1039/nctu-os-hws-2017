#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>
#include "pinfo.hpp"


int main(int argc, char *argv[])
{
	using prpr::info;
	std::fstream input("Q4.txt", std::ios_base::in);
	if (not input.is_open())
		return 1;
	int process_id(0), process_size, process_max_time(0), time_quantum_high_priority, time_quantum_medium_priority;
	input >> process_size;
	std::vector<info> waiting_process, high_priority_process, medium_priority_process, sjf_process;
	std::map<int, info> done_process;
	{
		std::vector<int> tmp_arrv, tmp_burst;
		while(process_id++ < process_size)
		{
			int arrv;
			input >> arrv;
			tmp_arrv.push_back(arrv);
		}
		process_id = 0;
		while(process_id++ < process_size)
		{
			int burst;
			input >> burst;
			process_max_time += burst;
			tmp_burst.push_back(burst);
		}
		input >> time_quantum_high_priority >> time_quantum_medium_priority;
		process_id = 0;
		while(process_id++ < process_size)
			waiting_process.push_back(info(process_id, tmp_arrv.at(process_id - 1), tmp_burst.at(process_id - 1)));
	}

	info current_process(0, 0, 0);
	enum LVL{ HIGH_PRIORITY, MEDIUM_PRIORITY, SJF };
	LVL level(HIGH_PRIORITY);
	for(int tick(0), time_quantum; tick <= process_max_time; tick++)
	{
		for(int j(0); j < waiting_process.size();)
		{
			if(waiting_process.at(j).start == tick)
			{
			    high_priority_process.push_back(waiting_process.at(j));
				waiting_process.erase(waiting_process.begin() + j);
			}
			else
				j++;
		}

		if(not high_priority_process.empty() && (level == MEDIUM_PRIORITY || level == SJF))
		{
			switch(level)
			{
    			case MEDIUM_PRIORITY:
    				medium_priority_process.push_back(current_process);
    				break;
    			case SJF:
    				sjf_process.push_back(current_process);
			}
			current_process = *high_priority_process.begin();
			high_priority_process.erase(high_priority_process.begin());
			level = HIGH_PRIORITY;
			time_quantum = time_quantum_high_priority;
		}
		else if(not medium_priority_process.empty() && level == SJF)
		{
			sjf_process.push_back(current_process);
			current_process = *medium_priority_process.begin();
			medium_priority_process.erase(medium_priority_process.begin());
			level = MEDIUM_PRIORITY;
			time_quantum = time_quantum_medium_priority;
		}

		if(current_process.is_done(tick))
		{
			done_process[current_process.id] = current_process;
			std::vector<info> *source = NULL;
			if(not high_priority_process.empty())
			{
				source		 = &high_priority_process;
				level		 = HIGH_PRIORITY;
				time_quantum = time_quantum_high_priority;
			}
			else if(not medium_priority_process.empty())
			{
				source	     = &medium_priority_process;
				level		 = MEDIUM_PRIORITY;
				time_quantum = time_quantum_medium_priority;
			}
			else
			{
				std::sort(sjf_process.begin(), sjf_process.end());
				source = &sjf_process;
				level  = SJF;
			}


			if(not source->empty())
			{
				current_process = *source->begin();
			    source->erase(source->begin());
			}
		}
		else if(time_quantum == 0) // times up!
		{
			switch(level)
			{
			    case HIGH_PRIORITY:
			    {
			    	medium_priority_process.push_back(current_process);
					if(not high_priority_process.empty())
					{
						current_process = *high_priority_process.begin();
						high_priority_process.erase(high_priority_process.begin());
						time_quantum = time_quantum_high_priority;
						level = HIGH_PRIORITY;
					}
					else
					{
						current_process = *medium_priority_process.begin();
						medium_priority_process.erase(high_priority_process.begin());
						time_quantum = time_quantum_medium_priority;
						level = MEDIUM_PRIORITY;
					}
					break;
			    }
			    case MEDIUM_PRIORITY:
			    {
			    	sjf_process.push_back(current_process);
					if(not high_priority_process.empty())
					{
						current_process = *high_priority_process.begin();
						high_priority_process.erase(high_priority_process.begin());
						time_quantum = time_quantum_high_priority;
						level = HIGH_PRIORITY;
					}
					else if(not medium_priority_process.empty())
					{
						current_process = *medium_priority_process.begin();
						medium_priority_process.erase(medium_priority_process.begin());
						time_quantum = time_quantum_medium_priority;
						level = MEDIUM_PRIORITY;
					}
					else
					{
						std::sort(sjf_process.begin(), sjf_process.end());
						current_process = *sjf_process.begin();
						sjf_process.erase(sjf_process.begin());
						level = SJF;
					}
					break;
			    }
			}
		}
	    current_process.use_cpu(tick);
		time_quantum--;
//		std::cout << tick << "\t" << current_process.id << "\n";
	}

	std::cout << "Process\tWaiting Time\tTurnaround Time\n";
	double total_waiting_time(0), total_turnaround_time(0);
	for(int i(1); i <= process_size; i++)
	{
		//done_process[i].show();
		std::cout << "P["<<done_process[i].id << "]\t"
				  << done_process[i].waiting() << "\t\t\t\t"
				  << done_process[i].turnaround() << std::endl;
		total_waiting_time += done_process[i].waiting();
		total_turnaround_time += done_process[i].turnaround();
	}
	std::cout << "Average waiting time :\t\t" << total_waiting_time / process_size << "\n"
			  << "Average turnaround time :\t" << total_turnaround_time / process_size << "\n";
	return 0;
}
