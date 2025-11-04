#pragma once

#include "IWorkerAction.h"
#include "IETThread.h"

class IFinishedTask {
	public:
		virtual void OnFinishedTask(int id) = 0;

};

class WorkerThread : public IETThread {
public:
	WorkerThread(int _id, IFinishedTask* _onDone);
	~WorkerThread();

	void AssignTask(IWorkerAction* _task);
	int GetId() { return id; }

private:
	void run() override;
	int id;
	IFinishedTask* onDone;
	IWorkerAction* task;
};