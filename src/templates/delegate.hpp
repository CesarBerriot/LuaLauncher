//
// Created by César B. on 3/21/2024.
//

#pragma once

#include <functional>
#include <vector>
#include <assert.h>

namespace templates
{
	template<typename>
	class delegate { public: delegate() { assert(!"the given type was not a function pointer"); } };
	template<typename _ret, typename... _args>
	class delegate<_ret(_args...)>
	{	typedef _ret(_func)(_args...);
		private:
			std::vector<std::function<_func>> bound_functions;
		public:
			void bind(std::function<_func> func) { bound_functions.push_back(func); }
			template<typename _class>
			void bind(_class object, std::function<_func> method) { bound_functions.push_back(std::bind(method, object)); }
			void broadcast(_args... args) { for(auto func : bound_functions) func(args...); }
	};
}
