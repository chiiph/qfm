#ifndef RENAMECOMMAND_H
#define RENAMECOMMAND_H

#include "commandbuffer.h"
#include "movecommand.h"

#include <QtGui>

class RenameCommand : public CommandBuffer {
	public:
		RenameCommand();
		virtual ~RenameCommand();
	
	public slots:
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);
		virtual bool show();
	
	private:
		QString newname;

		friend MoveCommand::run(QString current_dir, QStringList files);
};

#include "ui/ui_renameprompt.h"

class RenamePrompt : public QDialog {
	public:
		RenamePrompt(QWidge *parent = 0);
		~RenamePrompt();

		static QString prompt();

	private:
		// Ui definition of the mainwindow
		Ui::renameprompt ui;
};

#endif // PASTECOMMAND_H
