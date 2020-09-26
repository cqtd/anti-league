#ifndef _LOOP_H_
#define _LOOP_H_

#pragma once
#include "pch.h"

#include <chrono>
#include <thread>

namespace core
{
	namespace loop
	{
		constexpr unsigned int update_delay = 40;
		constexpr unsigned int fixed_update_delay = 100;
		constexpr unsigned int late_update_delay = 1000;
		
		LPTHREAD_START_ROUTINE update();
		LPTHREAD_START_ROUTINE fixed_update();
		LPTHREAD_START_ROUTINE late_update();

		void menu_key_bind();
	}
};

#endif