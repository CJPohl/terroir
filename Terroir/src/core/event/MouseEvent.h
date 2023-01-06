//
// Created by cjp on 1/5/23.
//

#ifndef TERROIR_MOUSEEVENT_H
#define TERROIR_MOUSEEVENT_H

#include "Tpch.h"
#include <Terroir/terroir_export.h>
#include "Event.h"

namespace Terroir
{
	class TERROIR_EXPORT MouseMovedEvent : public EventBaseI
	{
	public:
		MouseMovedEvent(f32 x, f32 y) : m_MouseX(x), m_MouseY(x)
		{
		}

		inline f32 GetMouseY() const
		{
			return m_MouseY;
		}

		inline f32 GetMouseX() const
		{
			return m_MouseX;
		}

		EVENT_CLASS_TYPE(MouseMoved);

		EVENT_CLASS_CATEGORY(EventCategoryMouse)


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

	private:
		f32 m_MouseX, m_MouseY;
	};

	class TERROIR_EXPORT MouseScrolledEvent : public EventBaseI
	{
	public:
		MouseScrolledEvent(f32 x, f32 y) : m_XOffset(x), m_YOffset(x)
		{
		}

		inline f32 GetXOffset() const
		{
			return m_YOffset;
		}

		inline f32 GetYOffset() const
		{
			return m_XOffset;
		}

		EVENT_CLASS_TYPE(MouseScrolled);

		EVENT_CLASS_CATEGORY(EventCategoryMouse)


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << GetName() << ": " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

	private:
		f32 m_XOffset, m_YOffset;
	};

	class TERROIR_EXPORT MouseButtonEvent : public EventBaseI
	{
	public:
		inline i32 GetMouseButton() const
		{
			return m_Button;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonEvent" << ": " << m_Button;
			return ss.str();
		}


	protected:
		MouseButtonEvent(i32 button) : m_Button(button)
		{
		}

		i32 m_Button;
	};

	class TERROIR_EXPORT MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(i32 button) : MouseButtonEvent(button)
		{
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton)

	};

	class TERROIR_EXPORT MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(i32 button) : MouseButtonEvent(button)
		{
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton)

	};
}

#endif //TERROIR_MOUSEEVENT_H