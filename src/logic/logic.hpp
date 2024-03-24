//
// Created by César B. on 3/21/2024.
//

#pragma once

namespace logic
{
	void bind_ui_elements();
	void refresh_script_list();
	/// @param script_name optional, used for logging purposes
	void run_code(std::string code, bool use_executor_log = false, std::string script_name = {});
	void run_script(std::string file_name);
	void run_executor_code();
	void clear_executor_code();
	void clear_executor_log();
}
