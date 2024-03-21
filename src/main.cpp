#include <QApplication>
#include "ui/ui.hpp"
#include "logic/logic.hpp"

int main(int argc, char * argv[])
{	QApplication application(argc, argv);
	ui::widgets::main_window::init();
	ui::widgets::main_window::main_window->show();
	logic::bind_ui_elements();
	logic::refresh_script_list();
	return application.exec();
}
