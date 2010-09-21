#ifndef GOTOCOMMAND_H
#define GOTOCOMMAND_H

#include "commandbuffer.h"

#include <QtGui>

class GotoCommand : public QObject, public CommandBuffer {
	Q_OBJECT
	Q_INTERFACES(CommandBuffer)
	public:
		GotoCommand();
		virtual ~GotoCommand();
	
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);
	
	signals:
		void go(QString str);
};

#include "ui/ui_gotoprompt.h"

class GotoPrompt : public QDialog {
	public:
		GotoPrompt(QString curdir, QWidget *parent = 0);
		~GotoPrompt();

		static QString prompt(QString path);

		void set_path(QString path) { ui.newpath_line->setText(path); }
		QString get_newpath() { return ui.newpath_line->text(); }

	private:
		Ui::gotoprompt ui;
		QCompleter *completer;
};

#endif // GOTOCOMMAND_H
