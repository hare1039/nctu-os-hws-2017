#ifndef __PINFO_HPP__
#define __PINFO_HPP__

#include <iostream>

namespace prpr
{
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
						  << "burst:            " << burst << std::endl << std::endl;
			}

		bool operator < (info const &b) const
			{
				return this->burst < b.burst;
			}
	};
}

#endif // __PINFO_HPP__
