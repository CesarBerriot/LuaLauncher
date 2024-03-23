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
	ui::widgets::main_window::central_widget::layout::script_list::item_double_clicked_cb.bind(io::run_script);
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

void logic::run_executor_code()
{	lua_State * L = nullptr;
	int original_stdout;
	FILE * redirected_stdout_file = nullptr;
	try
	{	// create lua virtual machine
		L = luaL_newstate();
		if(!L)
			throw "Failed to instantiate a Lua VM";

		// allow script access to all standard libraries
		luaL_openlibs(L);

		// redirect stdout
		redirected_stdout_file = utils::redirect_stdout(&original_stdout);

		// load code from the executor
		std::string code = ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->toPlainText().toStdString();
		if(luaL_loadstring(L, code.c_str()) != LUA_OK)
			throw make_string("Invalid Code Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// run the code
		ui::executor_log("Running The Script");
		if(lua_pcall(L, 0, 0, 0) != LUA_OK)
			throw make_string("Code Execution Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// read data from the redirected stdout
		enum { buflen = 1024 };
		char stdout_buffer[buflen];
		memset(stdout_buffer, '\0', buflen);
		fread(stdout_buffer, 1, buflen, redirected_stdout_file);

		// if non-empty, log the script output
		if(stdout_buffer[0])
		{	// if newline-terminated, remove the trailing newline character from the lua output
			if(int i = utils::find_last_eq_byte('\n', stdout_buffer, BUFSIZ); i != -1)
				stdout_buffer[i] = '\0';
			ui::executor_log(stdout_buffer, false);
		}
	}
	catch(std::string error_msg)
	{	ui::executor_log(error_msg);
	}

	// cleanup
	if(redirected_stdout_file)
		utils::restore_stdout(redirected_stdout_file, original_stdout);
	if(L)
	{	lua_settop(L, 0);
		lua_close(L);
	}
}

void logic::clear_executor_code()
{	ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->clear();
}

void logic::clear_executor_log()
{	ui::widgets::lua_executor::layout::log_layout::log->clear();
}
