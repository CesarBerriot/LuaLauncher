//
// Created by César B. on 3/21/2024.
//

#include <sstream>
#include <lua.hpp>
#include "logic.hpp"
#include "ui/ui.hpp"
#include "io/io.hpp"
#include "macros/macros.hpp"
#include "utils/utils.hpp"
#include <unistd.h>

void logic::bind_ui_elements()
{	// bind io::run_script() on script item double clicked
	ui::widgets::main_window::central_widget::layout::script_list::item_double_clicked_cb.bind(run_script);
	// bind logic::run_executor_code() on executor run button
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::run_button_clicked_cb.bind(run_executor_code);
	// bind code clear on executor clear button
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::clear_code_button_clicked_cb.bind(clear_executor_code);
	// bind executor log clear on executor clear button
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::clear_log_button_clicked_cb.bind(clear_executor_log);
}

void logic::refresh_script_list()
{	std::vector<std::string> script_list = io::obtain_script_list();
	// logs
	if(!script_list.size())
	{	ui::log("No scripts were found.");
		return;
	}
	for(std::string script : script_list)
		ui::log((std::stringstream() << "Found script '" << script << ".lua'").str());
	// refresh ui
	ui::widgets::main_window::central_widget::layout::script_list::refresh(script_list);
}

void logic::run_code(std::string code, bool use_executor_log, std::string script_name)
{	auto log_function = [&use_executor_log](std::string msg, bool use_timestamps = true)
	{	if(use_executor_log)
			ui::executor_log(msg, use_timestamps);
		else
			ui::log(msg, use_timestamps);
	};

	lua_State * L = nullptr;
	utils::redirected_stdout redirected_stdout;
	bool is_stdout_redirected = false;
	try
	{	// create lua virtual machine
		L = luaL_newstate();
		if(!L)
			throw "Failed to instantiate a Lua VM";

		// allow script access to all standard libraries
		luaL_openlibs(L);

		// redirect stdout
		redirected_stdout = utils::redirect_stdout();
		is_stdout_redirected = true;

		// load code from the executor
		if(luaL_loadstring(L, code.c_str()) != LUA_OK)
			throw make_string("Invalid Code Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// run the code
		log_function(script_name.length() ? make_string("Running Script '" << script_name << '\'') : "Running The Script");
		if(lua_pcall(L, 0, 0, 0) != LUA_OK)
			throw make_string("Code Execution Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// read data from the redirected stdout
		enum { buflen = 1024 };
		char stdout_buffer[buflen];
		utils::read_redirected_stdout(redirected_stdout, stdout_buffer, buflen);

		// if non-empty, log the script output
		if(stdout_buffer[0])
		{	// if newline-terminated, remove the trailing newline character from the lua output
			if(int i = utils::find_last_eq_byte('\n', stdout_buffer, BUFSIZ); i != -1)
				stdout_buffer[i] = '\0';
			log_function(stdout_buffer, false);
		}
	}
	catch(std::string error_msg)
	{	log_function(error_msg);
	}

	// cleanup
	if(is_stdout_redirected)
		utils::restore_stdout(redirected_stdout);
	if(L)
	{	lua_settop(L, 0);
		lua_close(L);
	}
}

void logic::run_script(std::string file_name)
{	std::string code = io::read_script(file_name);
	if(code.length())
		run_code(code);
}

void logic::run_executor_code()
{	run_code(
		ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->toPlainText().toStdString(),
		true
	);
}

void logic::clear_executor_code()
{	ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->clear();
}

void logic::clear_executor_log()
{	ui::widgets::lua_executor::layout::log_layout::log->clear();
}
