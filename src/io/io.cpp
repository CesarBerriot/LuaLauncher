#include "io.hpp"
#include "ui/ui.hpp"
#include "utils/utils.hpp"
#include "macros/macros.hpp"
#include <sstream>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <lua.hpp>

void io::run_script(std::string file_name)
{	// compute full script name, including extension
	std::string full_name = make_string(file_name << ".lua");

	// logs
	ui::log(make_string("Running Script '" << full_name << '\''));

	// obtain full path of script
	std::string full_path = make_string(get_full_scripts_path() << "\\" << full_name);

	// instantiate a lua VM
	lua_State * L = luaL_newstate();
	if(!L)
	{	ui::log("Failed to instantiate a Lua VM");
		return;
	}
	luaL_openlibs(L);

	// load the file onto the VM's stack
	if(luaL_loadfile(L, full_path.c_str()) != LUA_OK)
	{	ui::log(make_string("Failed to load script file '" << full_path << "' : '"
							<< lua_tolstring(L, lua_gettop(L), nullptr) << '\''));
		goto cleanup;
	}

	// enable buffering of stdout
	char stdout_buffer[BUFSIZ];
	memset(stdout_buffer, '\0', BUFSIZ);
	setvbuf(stdout, stdout_buffer, _IOLBF, BUFSIZ);

	// run the script
	if(lua_pcall(L, 0, 0, 0) != LUA_OK)
	{	ui::log(make_string("Error while running script '" << full_path << "' : " << lua_tolstring(L, lua_gettop(L), nullptr)));
		lua_pop(L, lua_gettop(L));
		goto cleanup;
	}

	// remove newline from the end of the output, if present
	if(int i = utils::find_last_eq_byte('\n', stdout_buffer, BUFSIZ); i != -1)
		stdout_buffer[i] = '\0';

	// log the output & move stdout's cursor
	ui::log(stdout_buffer, false);

	// cleanup
cleanup:
	// disable stdout buffering
	setvbuf(stdout, NULL, _IONBF, 0);
	// clear Lua VM stack
	lua_settop(L, 0);
	// destroy the VM
	lua_close(L);
}

std::vector<std::string> io::obtain_script_list()
{	// obtain full path of scripts directory
	std::string scripts_path = get_full_scripts_path();

	// open scripts folder
	DIR * dir_stream = opendir(scripts_path.c_str());
	if(!dir_stream)
	{	ui::log("Folder 'scripts' doesn't exist !");
		return {};
	}

	// obtain lua file list
	std::vector<std::string> result;
	dirent * current_entry;
	while(current_entry = readdir(dir_stream))
	{	// sub-string len, length of ".lua"
		enum { slen = 4 };

		// can't contain .lua
		if(current_entry->d_namlen < 4)
			continue;

		// doesn't contain .lua
		if(strcmp(".lua", &current_entry->d_name[current_entry->d_namlen - slen]))
			continue;

		// remove file extension
		std::string file_name(current_entry->d_name);
		file_name.resize(file_name.size() - slen);
		// add this entry to the result vector
		result.push_back(file_name);
	}
	return result;
}

std::string io::get_full_scripts_path()
{	char cwd_buffer[PATH_MAX];
	getcwd(cwd_buffer, PATH_MAX);
	return make_string(cwd_buffer << "\\scripts");
}
