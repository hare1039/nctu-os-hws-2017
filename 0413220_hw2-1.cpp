#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdio>

struct info
{
	int id, start, burst, finished, at_tick_start, at_tick_end;
	info():id(0),
		   start(0),
		   burst(0),
		   finished(0),
		   at_tick_start(0),
		   at_tick_end(0){}
	info(int id, int start, int burst): id(id),
										start(start),
										burst(burst),
										finished(0),
										at_tick_start(0),
										at_tick_end(0){}

	bool is_done(int at_tick)
	{
		if(finished == burst)
			at_tick_end = at_tick;
		return finished >= burst;
	}

	void use_cpu(int at_tick)
	{
		if(finished == 0)
			at_tick_start = at_tick;
		finished++;
	}
	int waiting() {return at_tick_end - start - burst;}
	int turnaround() {return at_tick_end - start;}

	void show()
	{
		std::cout << "id                " << id << "\n"
				  << "want to start at: " << start << "\n"
				  << "really start at:  " << at_tick_start << "\n"
				  << "really end at:    " << at_tick_end << "\n"
				  << "burst:            " << burst << std::endl;
	}
};

int main(int argc, char *argv[])
{
	std::fstream input("Q1.txt", std::ios_base::in);
	if (not input.is_open())
		return 1;
	int process_id(0), process_size, process_max_time(0);
	input >> process_size;
	std::vector<info> waiting_process, ready_process;
	std::map<int, info> done_process;
	while(process_id++ <= process_size)
	{
		int burst;
		input >> burst;
		process_max_time += burst;
		waiting_process.push_back(info(process_id, 0, burst));
	}

	info current_process(0, 0, 0);
	for(int tick(0); tick <= process_max_time; tick++)
	{
		for(int j(0); j < waiting_process.size();)
		{
			if(waiting_process.at(j).start == tick)
			{
				ready_process.push_back(waiting_process.at(j));
				waiting_process.erase(waiting_process.begin() + j);
			}
			else
				j++;
		}

		if(current_process.is_done(tick))
		{
			done_process[current_process.id] = current_process;
			if(not ready_process.empty())
			{
				current_process = *ready_process.begin();
				ready_process.erase(ready_process.begin());
			}
		}
		current_process.use_cpu(tick);
	}

	std::cout << "Process\tWaiting Time\tTurnaround Time\n";
	double total_waiting_time(0), total_turnaround_time(0);
	for(int i(1); i <= process_size; i++)
	{
		//done_process[i].show();
		std::cout << done_process[i].id << "\t\t" 
				  << done_process[i].waiting() << "\t\t\t\t"
				  << done_process[i].turnaround() << std::endl;
		total_waiting_time += done_process[i].waiting();
		total_turnaround_time += done_process[i].turnaround();
	}
	std::cout << "Average waiting time :\t\t" << total_waiting_time / process_size << "\n"
			  << "Average turnaround time :\t" << total_turnaround_time / process_size << "\n";
	return 0;
}
