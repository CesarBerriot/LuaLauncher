//
// Created by César B. on 3/21/2024.
//

#include <sstream>
#include "logic.hpp"
#include "ui/ui.hpp"
#include "io/io.hpp"

void logic::bind_ui_elements()
{	// bind io::run_script() on script item double clicked
	ui::widgets::main_window::central_widget::layout::script_list::item_double_clicked_cb.bind(io::run_script);
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
