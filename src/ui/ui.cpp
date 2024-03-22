//
// Created by César B. on 3/21/2024.
//

#include "ui.hpp"
#include <time.h>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>

void ui::widgets::main_window::init()
{	main_window = new QMainWindow();
	main_window->setWindowTitle("Lua Launcher");
	main_window->resize(800, 600);
	// menu bar
	menu_bar::init();
	main_window->setMenuBar(menu_bar::menu_bar);
	// central widget
	central_widget::init();
	main_window->setCentralWidget(central_widget::central_widget);
	// status bar
	status_bar = new QStatusBar(main_window);
	main_window->setStatusBar(status_bar);
	// logs
	log("Successfully Initialized Lua Launcher");
	for(int i = 0; i < 40; ++i)
		central_widget::layout::script_list::create_item("ntr");
}

void ui::widgets::main_window::menu_bar::init()
{	menu_bar = new QMenuBar(main_window);
	file_submenu::init();
	menu_bar->addAction(file_submenu::file_submenu->menuAction());
	help_submenu::init();
	menu_bar->addAction(help_submenu::help_submenu->menuAction());
}

void ui::widgets::main_window::menu_bar::file_submenu::init()
{	file_submenu = new QMenu(menu_bar);
	file_submenu->setTitle("File");
	// add
	add_action = new QAction(main_window);
	add_action->setText("Add");
	file_submenu->addAction(add_action);
}

void ui::widgets::main_window::menu_bar::help_submenu::init()
{	help_submenu = new QMenu(menu_bar);
	help_submenu->setTitle("Help");
	// about
	about_action = new QAction(main_window);
	about_action->setText("About");
	help_submenu->addAction(about_action);
}

void ui::widgets::main_window::central_widget::init()
{	central_widget = new QWidget(main_window);
	layout::init();
}

void ui::widgets::main_window::central_widget::layout::init()
{	layout = new QVBoxLayout(central_widget);
	// script list
	script_list::init();
	layout->addWidget(script_list::label);
	layout->addWidget(script_list::script_list);
	// log
	log::init();
	layout->addWidget(log::label);
	layout->addWidget(log::log);
}

void ui::widgets::main_window::central_widget::layout::script_list::init()
{	// label
	label = new QLabel(central_widget);
	label->setText("Scripts");
	// script list
	script_list = new QListWidget(central_widget);
	// *INDENT-OFF*
	QObject::connect(
		central_widget::layout::script_list::script_list,
		&QListWidget::itemDoubleClicked,
		[](QListWidgetItem * item) {item_double_clicked_cb.broadcast(item->text().toStdString());}
	);
	// *INDENT-ON*
}

void ui::widgets::main_window::central_widget::layout::script_list::create_item(std::string name)
{	QListWidgetItem * item = new QListWidgetItem();
	item->setText(name.c_str());
	script_list->addItem(item);
}

void ui::widgets::main_window::central_widget::layout::script_list::refresh(std::vector<std::string> new_items)
{	script_list->clear();
	for(std::string item_name : new_items)
		create_item(item_name);
}

void ui::widgets::main_window::central_widget::layout::log::init()
{	// label
	label = new QLabel(central_widget);
	label->setText("Log");
	// log
	log = new QTextBrowser(central_widget);
}

void ui::log(std::string msg, bool use_timestamps)
{	std::stringstream result_stream;
	if(use_timestamps)
	{	// get current time data
		time_t current_time = time(NULL);
		struct tm * time_info = localtime(&current_time);

		// format time data into a buffer
		enum { formatted_time_len = 12 };
		char formatted_time[formatted_time_len];
		strftime(formatted_time, formatted_time_len, "[%H:%M:%S] ", time_info);

		// concat **result_stream**
		result_stream << formatted_time;
	}

	result_stream << msg;

	ui::widgets::main_window::central_widget::layout::log::log->append(QString::fromStdString(result_stream.str()));
}
