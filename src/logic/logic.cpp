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
{	std::string code;

	// create lua virtual machine
	lua_State * L = luaL_newstate();
	if(!L)
	{	ui::executor_log("Failed to instantiate a Lua VM");
		goto cleanup;
	}

	// allow script access to all standard libraries
	luaL_openlibs(L);

	// enable stdout buffering
	char stdout_buffer[BUFSIZ];
	memset(stdout_buffer, '\0', BUFSIZ);
	setvbuf(stdout, stdout_buffer, _IOLBF, BUFSIZ);

	// load code from the executor
	code = ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->toPlainText().toStdString();
	if(luaL_loadstring(L, code.c_str()) != LUA_OK)
	{	ui::executor_log(make_string("Invalid Code Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\''));
		goto cleanup;
	}

	// run the code
	ui::executor_log("Running The Script");
	if(lua_pcall(L, 0, 0, 0) != LUA_OK)
	{	ui::executor_log(make_string("Code Execution Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\''));
		goto cleanup;
	}

	// if non-empty, log the script output
	if(size_t len = strlen(stdout_buffer))
	{	// if newline-terminated, remove the trailing newline character from the lua output
		if(int i = utils::find_last_eq_byte('\n', stdout_buffer, BUFSIZ); i != -1)
			stdout_buffer[i] = '\0';
		ui::executor_log(stdout_buffer, false);
	}

cleanup:
	setvbuf(stdout, nullptr, _IONBF, 0);
	lua_settop(L, 0);
	lua_close(L);
}

void logic::clear_executor_code()
{	ui::widgets::lua_executor::layout::top_layout::editor_layout::editor->clear();
}

void logic::clear_executor_log()
{	ui::widgets::lua_executor::layout::log_layout::log->clear();
}
