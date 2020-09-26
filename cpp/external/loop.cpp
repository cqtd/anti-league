#include "loop.h"

namespace core
{
	namespace loop
	{
		using namespace std;
		
		LPTHREAD_START_ROUTINE update()
		{
			while(true)
			{
				menu_key_bind();
				
				// process here

				this_thread::sleep_for(chrono::milliseconds(update_delay));
			}
		}
		
		LPTHREAD_START_ROUTINE fixed_update()
		{
			while (true)
			{
				// process here

				this_thread::sleep_for(chrono::milliseconds(fixed_update_delay));
			}
		}
		
		LPTHREAD_START_ROUTINE late_update()
		{
			while (true)
			{
				// process here

				this_thread::sleep_for(chrono::milliseconds(late_update_delay));
			}
		}

		void menu_key_bind()
		{
			if (GetAsyncKeyState(VK_INSERT))
			{
				const auto windowLong = GetWindowLong(renderer::hWindow, GWL_EXSTYLE);

				if (!renderer::showMenu)
				{
					if (windowLong != (WS_EX_LAYERED | WS_EX_TOPMOST))
					{
						SetWindowLong(renderer::hWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
					}
				}

				if (renderer::showMenu)
				{
					if (windowLong != (WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT))
					{
						SetWindowLong(renderer::hWindow, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT);
					}
				}

				renderer::showMenu = !renderer::showMenu;

				while (GetAsyncKeyState(VK_INSERT)) {}
			}
		}
	}
}
