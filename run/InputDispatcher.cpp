#include "InputDispatcher.h"

InputDispatcher::InputDispatcher(RenderWindow* window)
{
	m_Window = window;
}

void InputDispatcher::dispatchInputEvents()
{
	while(const std::optional event = m_Window->pollEvent())
	{
		for(const auto& ir : m_InputReceivers)
		{
			if(event.has_value())
				ir->addEvent(event.value());
		}
	}
}

void InputDispatcher::registerNewInputReceiver(InputReceiver* ir)
{
	m_InputReceivers.push_back(ir);
}

