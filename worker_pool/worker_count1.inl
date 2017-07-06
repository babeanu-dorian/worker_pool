namespace wp
{
	template <typename Functor, class Container, Functor &(Container::*Next)(void),
	          void(Container::*Pop)(void), void(Container::*Push)(Functor&&)>
	void worker_pool<Functor, Container, Next, Pop, Push>::worker_count(size_t nWorkers)
	{
		std::lock_guard<std::mutex> countLock(d_countMut);
		std::unique_lock<std::mutex> workLock(d_workMut);
		size_t workerIdx = d_workers.size();

		// less workers
		if (nWorkers < workerIdx)
		{
			auto workerIt = d_workers.begin();
			std::advance(workerIt, nWorkers);

			// move unwanted workers into a different list
			std::list<std::thread> workersToStop;
			workersToStop.splice(workersToStop.begin(), d_workers, workerIt, d_workers.end());

			workLock.unlock();
			d_taskCond.notify_all(); // threads to be stopped might be waiting

			// join discarded threads
			for (auto it = workersToStop.begin(); it != workersToStop.end(); ++it)
				it->join();

			return;
		}

		// more workers
		for (; workerIdx < nWorkers; ++workerIdx)
			d_workers.emplace_back(&worker_pool::work, this, workerIdx);
	}
}