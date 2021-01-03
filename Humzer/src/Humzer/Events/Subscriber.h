#pragma once

#include <functional>

namespace Humzer {

	typedef void(SubscriptionFunction(void* eventData));
	typedef bool(CallingConditionFunction(double eventID));

	class Subscriber {
	public:
		Subscriber(void* owner);
		Subscriber(void* owner, double watchedEvent);
		Subscriber(Subscriber &other);

		bool DefaultCondition(double value);
		void DefaultAction(void* data);
	
		std::function<SubscriptionFunction> m_Method;
		std::function<CallingConditionFunction> m_CallingCondition;
	private:

		void* m_Owner;
		double m_EventID;

		friend class Dispatcher;
	};

}
