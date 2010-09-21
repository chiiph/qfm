#ifndef QFM_H
#define QFM_H

#include <QtGui>

#include "core/qfmcore.h"
#include "ui/ui_mainwin.h"

class ListItem;

class Qfm : public QMainWindow {
	Q_OBJECT

	public:
		Qfm();
		virtual ~Qfm();
	
	private slots:
		// Handles the list layouts
		void update_layouts();

	private:
		// Ui definition of the mainwindow
		Ui::MainWindow ui;
		// Spacers for the UI
		QSpacerItem *spacer_selected, *spacer_directory;
		// Qfm Core instance
		QfmCore *core;

		// Key event handler
		void keyPressEvent(QKeyEvent *ev);

		void up(QfmCore::Buffer b);
		void down(QfmCore::Buffer b);

		// Appends to ui.selected_layout
		// the file selected in position pos
		void select(int pos, QfmCore::Buffer src, QfmCore::Buffer dest);
		void move_selected(QfmCore::Buffer src, QfmCore::Buffer dest);
};

#endif // QFM
