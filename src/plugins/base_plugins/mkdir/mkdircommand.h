#ifndef MKDIRCOMMAND_H
#define MKDIRCOMMAND_H

#include "commandbuffer.h"

#include <QtGui>

class MkdirCommand : public QObject, public CommandBuffer {
	Q_OBJECT
	Q_INTERFACES(CommandBuffer)
	public:
		MkdirCommand();
		virtual ~MkdirCommand();
	
	public slots:
		virtual void run(QString current_dir, QStringList files);
	
	private:
		QString newname;
};

#include "ui/ui_mkdirprompt.h"

class MkdirPrompt : public QDialog {
	public:
		MkdirPrompt(QWidget *parent = 0);
		~MkdirPrompt();

		static QString prompt();

		QString get_newname() { return ui.newname_line->text(); }

	private:
		Ui::mkdirprompt ui;
};

#endif // MKDIRCOMMAND_H
