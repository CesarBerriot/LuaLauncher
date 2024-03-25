#include <QApplication>
#include "ui/ui.hpp"
#include "logic/logic.hpp"

int main(int argc, char * argv[])
{	// create qt application
	QApplication application(argc, argv);
	// init ui
	ui::init_all_widgets();
	// init logic
	logic::bind_ui_elements();
	logic::refresh_script_list();
	//show main window
	ui::widgets::main_window::main_window->show();
	// run
	return application.exec();
}
