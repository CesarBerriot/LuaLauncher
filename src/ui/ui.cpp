//
// Created by César B. on 3/21/2024.
//

#include "ui.hpp"
#include <time.h>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QDialog>

#define make_submenu_action(submenu, name, label)	\
	do	\
	{	name##_action = new QAction(main_window); \
		name##_action->setText(label); \
		QObject::connect(name##_action, &QAction::triggered, [] { name##_action_clicked_cb.broadcast(); }); \
		submenu##_submenu->addAction(name##_action); \
	} while(false)

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
}

void ui::widgets::main_window::menu_bar::init()
{	menu_bar = new QMenuBar(main_window);
	// file
	file_submenu::init();
	menu_bar->addAction(file_submenu::file_submenu->menuAction());
	// view
	view_submenu::init();
	menu_bar->addAction(view_submenu::view_submenu->menuAction());
	// help
	help_submenu::init();
	menu_bar->addAction(help_submenu::help_submenu->menuAction());
}

void ui::widgets::main_window::menu_bar::file_submenu::init()
{	file_submenu = new QMenu(menu_bar);
	file_submenu->setTitle("File");
	make_submenu_action(file, add, "Add");
	make_submenu_action(file, exit, "Exit");
}

void ui::widgets::main_window::menu_bar::view_submenu::init()
{	view_submenu = new QMenu(menu_bar);
	view_submenu->setTitle("View");
	make_submenu_action(view, refresh, "Refresh");
	make_submenu_action(view, lua_executor, "Lua Executor");
}

void ui::widgets::main_window::menu_bar::help_submenu::init()
{	help_submenu = new QMenu(menu_bar);
	help_submenu->setTitle("Help");
	make_submenu_action(help, about, "About");
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
		[](QListWidgetItem * item) { item_double_clicked_cb.broadcast(item->text().toStdString()); }
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

void ui::log(std::string msg, bool use_timestamps, QTextBrowser * log)
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

	log->append(QString::fromStdString(result_stream.str()));
}

void ui::init_all_widgets()
{	widgets::main_window::init();
	widgets::lua_executor::init();
	widgets::about_window::init();
}

void ui::widgets::lua_executor::init()
{	lua_executor = new QDialog;
	lua_executor->resize(600, 400);
	lua_executor->setWindowTitle("Lua Executor");
	lua_executor->setWindowFlag(Qt::WindowMaximizeButtonHint);
	lua_executor->setModal(true);
	layout::init();
	lua_executor->setLayout(layout::layout);
	executor_log("Successfully Initialized");
}

void ui::widgets::lua_executor::layout::init()
{	layout = new QVBoxLayout;
	top_layout::init();
	layout->addItem(top_layout::top_layout);
	log_layout::init();
	layout->addItem(log_layout::log_layout);
}

void ui::widgets::lua_executor::layout::top_layout::init()
{	top_layout = new QHBoxLayout;
	top_layout->setSpacing(11);
	editor_layout::init();
	top_layout->addItem(editor_layout::editor_layout);
	button_panel_layout::init();
	top_layout->addItem(button_panel_layout::button_panel_layout);
}

void ui::widgets::lua_executor::layout::top_layout::editor_layout::init()
{	editor_layout = new QVBoxLayout;
	//label
	label = new QLabel(lua_executor);
	label->setText("Editor");
	editor_layout->addWidget(label);
	//editor
	editor = new QTextEdit(lua_executor);
	editor_layout->addWidget(editor);
}

void ui::widgets::lua_executor::layout::top_layout::button_panel_layout::init()
{	button_panel_layout = new QVBoxLayout;
	// label
	label = new QLabel("Actions", lua_executor);
	label->setMinimumWidth(150);
	button_panel_layout->addWidget(label);
	// run button
	run_button = new QPushButton("Run", lua_executor);
	QObject::connect(run_button, &QPushButton::clicked, [] { run_button_clicked_cb.broadcast(); });
	button_panel_layout->addWidget(run_button);
	// clear code button
	clear_code_button = new QPushButton("Clear Code", lua_executor);
	button_panel_layout->addWidget(clear_code_button);
	QObject::connect(clear_code_button, &QPushButton::clicked, [] { clear_code_button_clicked_cb.broadcast(); });
	// clear log button
	clear_log_button = new QPushButton("Clear Log", lua_executor);
	button_panel_layout->addWidget(clear_log_button);
	QObject::connect(clear_log_button, &QPushButton::clicked, [] { clear_log_button_clicked_cb.broadcast(); });
	// spacer
	spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
	button_panel_layout->addItem(spacer);
}

void ui::widgets::lua_executor::layout::log_layout::init()
{	log_layout = new QVBoxLayout;
	label = new QLabel("Log", lua_executor);
	label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	log_layout->addWidget(label);
	log = new QTextBrowser(lua_executor);
	log->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	log->setMaximumHeight(150);
	log_layout->addWidget(log);
}

void ui::widgets::about_window::init()
{	about_window = new QDialog(main_window::main_window);
	about_window->setModal(true);
	about_window->setWindowTitle("About");
	// layout
	layout::init();
	about_window->setLayout(layout::layout);
	// lock the window's size
	layout::layout->setSizeConstraint(QLayout::SetFixedSize);
	enum { margin = 10 };
	about_window->setContentsMargins(margin, margin, margin, margin);
}

void ui::widgets::about_window::layout::init()
{	layout = new QVBoxLayout;
	label = new QLabel(about_window);
	label->setTextFormat(Qt::MarkdownText);
	//label->setStyleSheet("QLabel { color : blue; }");
	label->setOpenExternalLinks(true);
	label->setText(
		"# LuaLauncher  \n"
		"Made with ♥ by César Berriot  \n"
		"<a style=color:blue; href=\"https://www.linkedin.com/in/c%C3%A9sar-berriot/\">LinkedIn</a> • "
		"<a style=color:blue; href=\"https://github.com/CesarBerriot\">Github</a> • "
		"<a style=color:blue; href=\"https://github.com/CesarBerriot/LuaLauncher\">This project</a>"
	);
	layout->addWidget(label);
}
