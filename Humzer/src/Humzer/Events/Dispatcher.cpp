#include "humpch.h"
#include "Dispatcher.h"
#include "Subscriber.h"

namespace Humzer {

	bool inited;

	std::vector<std::pair<double, void*>>* dispatchEvents;
	std::list<Subscriber*>* subscriberQueue;
	std::map<int, std::list<Subscriber*>*>* mappedEvents;

	std::thread processingThread;

	double* localDeltaTime;

	bool running;
	bool processing;
	bool dispatchLock;
	bool subscriberLock;
	bool mappedLock;

	Dispatcher::Dispatcher() { }

	Dispatcher* Dispatcher::GetInstance() {
		Dispatcher theInstance;
		return &theInstance;
	}

	//Does not invalidate subscriber objects
	Dispatcher::~Dispatcher() {
		if (subscriberQueue) delete subscriberQueue;

		if (dispatchEvents)	delete dispatchEvents;

		if (mappedEvents) {
			std::list<int> lists;
			for (auto pair : *mappedEvents) {
				lists.push_back(pair.first);
			}

			for (auto num : lists) delete mappedEvents->at(num);
			delete mappedEvents;
		}

		running = false;
		mappedLock = false;
		dispatchLock = false;
		subscriberLock = false;

		//if(processingThread.joinable()) processingThread.join();
	}

	void Dispatcher::Initialize() {
		inited = true;
		subscriberQueue = new std::list<Subscriber*>();
		dispatchEvents = new std::vector<std::pair<double, void*>>();
		mappedEvents = new std::map<int, std::list<Subscriber*>*>();

		running = true;
		processing = true;
		dispatchLock = false;
		mappedLock = false;
		subscriberLock = false;


		localDeltaTime = (double*)0;
		processingThread = std::thread(Process, localDeltaTime); //starts the processing thread
		processingThread.detach(); // it probably won't terminate before the end of this program so we want to ignore errors
	}

	void Dispatcher::Process(double* deltaTime) {
		while (running) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));

			while (processing && running) {
				if (!dispatchLock) {
					if (dispatchEvents->size() == 0) { // if no events have been dispatched, wait a little longer
						std::this_thread::sleep_for(std::chrono::milliseconds(2));
						continue;
					}
					//events have been dispatched so force other threads to block when dispatching new events
					dispatchLock = true;

					//get the first event pair
					std::pair<double, void*> eventPair = std::pair<double, void*>(dispatchEvents->at(dispatchEvents->size() - 1).first, dispatchEvents->at(dispatchEvents->size() - 1).second);
					dispatchEvents->pop_back();
					//copy constructor for thread safety
					dispatchLock = false;		//and so we can unlock the list so more can be dispatched

					//wait to acquire the map lock
					while (mappedLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
					if (!processing) break;
					mappedLock = true;

					//got the lock but only lock the subscribers if we need to
					if (mappedEvents->count(eventPair.first) > 0) {
						// if subscribers are listening for this event specifically
						//we need to acquire the subscriber lock too
						while (subscriberLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
						if (!processing) { mappedLock = false; break; }
						subscriberLock = true;
						//got it

						for (auto subscriber : *mappedEvents->at(eventPair.first)) {
							if (!subscriber) { // warn of null subscribers
								HUM_CORE_WARN("Dispatcher --->  Null subscriber in SpecificEventSubscribers.");
								continue;
							}
							//if the subscriber has a method to be called then call it
							if (subscriber && subscriber->m_Method)
								subscriber->m_Method(eventPair.second);
						}
					}
					//we're done with the map so unlock it
					mappedLock = false;

					for (auto subscriber : (*subscriberQueue)) {
						if (!subscriber) {
							HUM_CORE_WARN("Dispatcher --->  Null subscriber in EventSubscriberQueue.");
							continue;
						}
						//if the calling condition returns true, call the method
						if (subscriber->m_CallingCondition != nullptr && subscriber->m_CallingCondition(eventPair.first)) {
							subscriber->m_Method(eventPair.second);
						}
						else if (subscriber->m_CallingCondition == nullptr) HUM_CORE_WARN("Dispatcher --->  Null calling condition in general subscriber object for event_id {0}.", eventPair.first);
					}//else warn about a null calling condition

					//done with subscribers now
					subscriberLock = false;
				}
			}
		}
		//Something forced us out of our loop
		HUM_CORE_WARN("Processing thread terminated unexpectedly");
	}

	void Dispatcher::DispatchEvent(double eventID, void* eventData) {
		HUM_CORE_TRACE("Dispatcher ---> Received event with ID: {0}.", eventID);

		dispatchEvents->push_back(std::pair<double, void*>(eventID, eventData));
	}

	void Dispatcher::AddEventSubscriber(Subscriber* requestor) {
		while (subscriberLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		subscriberLock = true;
		subscriberQueue->push_back(requestor); //allow duplicates
		subscriberLock = false;
	}

	void Dispatcher::AddSpecificEventSubscriber(Subscriber* requestor, double event_id) {
		if (mappedEvents->count(event_id) > 0) {
			mappedEvents->at(event_id)->push_back(requestor);
		}
		else {
			HUM_CORE_WARN("Dispatcher --->  Dynamically allocating list for Specific EventID {0}. (This should be avoided for performance reasons.)", event_id);
			mappedEvents->emplace(event_id, new std::list<Subscriber*>());
			mappedEvents->at(event_id)->push_back(requestor);
		}
	}

	Subscriber* Dispatcher::RemoveEventSubscriber(Subscriber* requestor) {
		while (subscriberLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		subscriberLock = true;


		auto list = std::list<Subscriber*>();

		for (auto sub : (*subscriberQueue)) {
			list.push_back(sub);
		}

		for (auto sub : list) {
			subscriberQueue->remove(sub);
		}

		subscriberLock = false;

		return requestor;
	}

	Subscriber* Dispatcher::RemoveSpecificEventSubscriber(Subscriber* requestor, double event_id) {
		while (mappedLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		mappedLock = true;

		if (mappedEvents->find(event_id) != mappedEvents->end()) {

			while (subscriberLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
			subscriberLock = true;

			auto list = std::list<Subscriber*>();
			Subscriber* s = nullptr;

			for (auto sub : *mappedEvents->at(event_id)) {
				if (sub == requestor) list.push_back(sub);
			}

			for (auto sub : list) {
				s = sub;
				mappedEvents->at(event_id)->remove(sub);
			}
			mappedLock = false;
			subscriberLock = false;
			return s;
		}
		return nullptr;
	}

	//Does not include the SpecificEventSubscriber
	std::list<Subscriber*> Dispatcher::GetAllSubscribers(void* owner) {
		std::list<Subscriber*> tmp;

		while (subscriberLock && processing) std::this_thread::sleep_for(std::chrono::milliseconds(1));
		subscriberLock = true;

		for (Subscriber* sub : (*subscriberQueue)) {
			if (sub->m_Owner == owner) tmp.push_back(sub);
		}
		subscriberLock = false;
		return tmp;
	}

	void Dispatcher::Stop() {
		processing = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	void Dispatcher::Start() {
		processing = true;
	}

	void Dispatcher::Terminate() {
		running = false;
		processing = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}
