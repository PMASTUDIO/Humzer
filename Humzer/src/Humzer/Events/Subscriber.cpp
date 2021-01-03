#include "humpch.h"
#include "Subscriber.h"

namespace Humzer {

	Subscriber::Subscriber(void* owner) : m_Owner(owner)
	{
		m_CallingCondition = std::bind(&Subscriber::DefaultCondition, this, 0.0);
		m_Method = std::bind(&Subscriber::DefaultAction, this, nullptr);
	}

	Subscriber::Subscriber(void* owner, double watchedEvent) : m_Owner(owner)
	{
		m_EventID = watchedEvent;

		m_Method = std::bind(&Subscriber::DefaultAction, this, nullptr);
	}

	Subscriber::Subscriber(Subscriber& other) 
		: m_Owner(other.m_Owner), m_CallingCondition(other.m_CallingCondition), m_Method(other.m_Method), m_EventID(other.m_EventID)
	{
		
	}

	bool Subscriber::DefaultCondition(double value)
	{
		return true;
	}

	void Subscriber::DefaultAction(void* data)
	{
		HUM_CORE_WARN("DefaultAction for event {0} not overriden in subscriber object.", m_EventID);
	}

}

