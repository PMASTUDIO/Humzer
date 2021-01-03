#pragma once
#include "Subscriber.h"

namespace Humzer {

	class Dispatcher {

	public:
		Dispatcher();
		~Dispatcher();

		static Dispatcher* GetInstance();

		Dispatcher(const Dispatcher&) = delete; //disallow copying
		Dispatcher& operator= (const Dispatcher&) = delete; //disallow copying

		static void Initialize();

		static void Start();
		static void Stop();
		static void Terminate();

		static void AddEventSubscriber(Subscriber* requestor);
		static void AddSpecificEventSubscriber(Subscriber* requestor, double event_id);
		static Subscriber* RemoveSpecificEventSubscriber(Subscriber* requestor, double event_id);
		static Subscriber* RemoveEventSubscriber(Subscriber* requestor);
		static std::list<Subscriber*> GetAllSubscribers(void* owner);

		static void DispatchEvent(double eventID, void* eventData);

	private:
		static void Process(double* deltaTime);
	};

}
