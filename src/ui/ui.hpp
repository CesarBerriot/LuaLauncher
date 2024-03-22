//
// Created by César B. on 3/21/2024.
//

#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <string>
#include <sstream>
#include "templates/delegate.hpp"

namespace ui
{	namespace widgets
	{	namespace main_window
		{	void init();
			inline QMainWindow * main_window;
			namespace menu_bar
			{	void init();
				inline QMenuBar * menu_bar;
				namespace file_submenu
				{	void init();
					inline QMenu * file_submenu;
					inline QAction * add_action;
				}
				namespace view_submenu
				{	void init();
					inline QMenu * view_submenu;
					inline QAction * lua_executor_action;
				}
				namespace help_submenu
				{	void init();
					inline QMenu * help_submenu;
					inline QAction * about_action;
				}
			}
			namespace central_widget
			{	void init();
				inline QWidget * central_widget;
				namespace layout
				{	void init();
					inline QVBoxLayout * layout;
					namespace script_list
					{	void init();
						void create_item(std::string name);
						void refresh(std::vector<std::string> new_items);
						inline QLabel * label;
						inline QListWidget * script_list;
						inline templates::delegate<void(std::string)> item_double_clicked_cb;
					}
					namespace log
					{	void init();
						inline QLabel * label;
						inline QTextBrowser * log;
					}
				}
			}
			inline QStatusBar * status_bar;
		}
		namespace lua_executor
		{	void init();
			inline QWidget * lua_executor;
			namespace layout
			{	void init();
				inline QVBoxLayout * layout;
				namespace top_layout
				{	void init();
					inline QHBoxLayout * top_layout;
					namespace editor_layout
					{	void init();
						inline QVBoxLayout * editor_layout;
						inline QLabel * label;
						inline QTextEdit * editor;
					}
					namespace button_panel_layout
					{	void init();
						inline QVBoxLayout * button_panel_layout;
						inline QLabel * label;
						inline QPushButton * run_button;
						inline QPushButton * clear_code_button;
						inline QPushButton * clear_log_button;
						inline QSpacerItem * spacer;
						inline templates::delegate<void()> run_button_clicked_cb;
						inline templates::delegate<void()> clear_code_button_clicked_cb;
						inline templates::delegate<void()> clear_log_button_clicked_cb;
					}
				}
				namespace log_layout
				{	void init();
					inline QVBoxLayout * log_layout;
					inline QLabel * label;
					inline QTextBrowser * log;
				}
			}
		}
	}
	void log(std::string msg, bool use_timestamps = true, QTextBrowser * log = widgets::main_window::central_widget::layout::log::log);
	inline void executor_log(std::string msg, bool use_timestamps = true) { log(msg, use_timestamps, widgets::lua_executor::layout::log_layout::log); }
}
