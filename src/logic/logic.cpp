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
#include <QFileDialog>
#include <libgen.h>

void logic::bind_ui_elements()
{	// script item double clicked -> io::run_script()
	ui::widgets::main_window::central_widget::layout::script_list::item_double_clicked_cb.bind(run_script);
	// executor run button -> logic::run_executor_code()
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::run_button_clicked_cb.bind(run_executor_code);
	// executor clear button -> logic::clear_executor_code()
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::clear_code_button_clicked_cb.bind(clear_executor_code);
	// executor clear button -> logic::clear_executor_log()
	ui::widgets::lua_executor::layout::top_layout::button_panel_layout::clear_log_button_clicked_cb.bind(clear_executor_log);
	// File->Add ->  logic::add_new_scripts()
	ui::widgets::main_window::menu_bar::file_submenu::add_action_clicked_cb.bind(add_new_scripts);
	// File->Exit -> exit()
	ui::widgets::main_window::menu_bar::file_submenu::exit_action_clicked_cb.bind([] { exit(EXIT_SUCCESS); });
	// View->Refresh -> logic::refresh_script_list()
	ui::widgets::main_window::menu_bar::view_submenu::refresh_action_clicked_cb.bind(refresh_script_list);
	// View->LuaExecutor -> ui::widgets::lua_executor::lua_executor->show()
	ui::widgets::main_window::menu_bar::view_submenu::lua_executor_action_clicked_cb.bind([]
	{	ui::widgets::lua_executor::lua_executor->show();
	});
	// Help->About -> ui::widgets::about_window::about_window->show()
	ui::widgets::main_window::menu_bar::help_submenu::about_action_clicked_cb.bind([]
	{	ui::widgets::about_window::about_window->show();
	});
}

void logic::refresh_script_list()
{	std::vector<std::string> script_list = io::obtain_script_list();
	// logs
	if(!script_list.size())
	{	ui::log("No scripts were found.");
		return;
	}
	for(std::string script : script_list)
		ui::log(make_string("Found script '" << script << ".lua'"));
	// refresh ui
	ui::widgets::main_window::central_widget::layout::script_list::refresh(script_list);
}

void logic::run_code(std::string code, bool use_executor_log, std::string script_name)
{	// operator ""s
	using namespace std::string_literals;

	auto log_function = [&use_executor_log](std::string msg, bool use_timestamps = true)
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
			throw "Failed to instantiate a Lua VM"s;

		// allow script access to all standard libraries
		luaL_openlibs(L);

		// redirect stdout
		redirected_stdout = utils::redirect_stdout();
		is_stdout_redirected = true;

		// load code from the executor
		if(luaL_loadstring(L, code.c_str()) != LUA_OK)
			throw make_string("Invalid Code Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// run the code
		if(script_name.length())
			log_function(make_string("Running Script '" << script_name << ".lua'"));
		else
			log_function("Running The Script");
		if(lua_pcall(L, 0, 0, 0) != LUA_OK)
			throw make_string("Code Execution Error : '" << lua_tolstring(L, lua_gettop(L), nullptr) << '\'');

		// read data from the redirected stdout
		enum { buflen = 1024 };
		char stdout_buffer[buflen];
		utils::read_redirected_stdout(redirected_stdout, stdout_buffer, buflen);
		int stdout_buffer_str_len = utils::find_first_eq_byte('\0', stdout_buffer, buflen);
		if(stdout_buffer_str_len == -1)
			throw "Corrupted Stdout Read Buffer Error : Either the developer is absolutely fucking dumb"
			"or there's something very wrong with your system"s;

		// if non-empty, log the script output
		if(stdout_buffer_str_len)
		{	// if newline-terminated, remove the trailing newline character from the lua output
			int last_newline_index = utils::find_last_eq_byte('\n', stdout_buffer, stdout_buffer_str_len);
			if(last_newline_index != -1)
				stdout_buffer[last_newline_index] = '\0';
			// log the script output
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
		run_code(code, false, file_name);
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

void logic::add_new_scripts()
{	std::string full_scripts_path = io::get_full_scripts_path();

	// file dialog
	QList<QString> selected_files = QFileDialog::getOpenFileNames(
										ui::widgets::main_window::main_window,
										"Add Scripts",
										QString::fromStdString(full_scripts_path),
										"*.lua"
									);

	// copies
	for(QString selected_file : selected_files)
	{	// std::string copy_path = make_string(full_scripts_path << )
		std::string full_path = selected_file.toStdString();
		io::add_script(full_path);
	}

	// ui refresh
	logic::refresh_script_list();
}
