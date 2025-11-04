#pragma once

#include "IETThread.h"
#include "WorkerThread.h"
#include "IWorkerAction.h"

#include <queue>
#include <unordered_map>

class ThreadPool : public IETThread, public IFinishedTask {
public:
	ThreadPool(int _workerCount);
	~ThreadPool();

	void StartScheduling();
	void StopScheduling();

	void ScheduleTask(IWorkerAction* _task);

private:
	void run() override;
	void OnFinishedTask(int id) override;

	bool isRunning = false;
	int workerCount = 1;

	std::queue<IWorkerAction*> PendingTasks;
	std::unordered_map<int, WorkerThread*> ActiveThreads;
	std::queue<WorkerThread*> InactiveThreads;
};