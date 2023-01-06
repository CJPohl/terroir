//
// Created by cjp on 1/4/23.
//

#ifndef TERROIR_APPLICATION_H
#define TERROIR_APPLICATION_H

#include "Terroir/pch/Tpch.h"
#include <Terroir/terroir_export.h>
#include "../../platform/Window.h"
#include "../event/Event.h"
#include "../event/WindowEvent.h"

namespace Terroir
{
	class TERROIR_EXPORT Application
	{
	public:
		Application();

		Application(const std::string& name, u32 width, u32 height);

		virtual ~Application();

		void Init();

		void OnEvent(EventBaseI&);

	private:
		bool OnWindowClose(WindowCloseEvent&);

		std::unique_ptr<WindowBaseI> m_Window;
		bool m_Running = true;
	};

	// Client defined
	Application* CreateApplication();
}


#endif //TERROIR_APPLICATION_H