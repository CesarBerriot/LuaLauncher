//
// Created by César B. on 3/21/2024.
//

#pragma once

#include <QMainWindow>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QTextBrowser>
#include <string>
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
	}
	void log(std::string msg);
}
