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
};

#include "ui/ui_renameprompt.h"

class RenamePrompt : public QDialog {
	public:
		RenamePrompt(QWidget *parent = 0);
		~RenamePrompt();

		static QString prompt(QString file);

		void set_file(QString file) 
			{ ui.file_label->setText(file); ui.newname_line->setText(file); }
		QString get_newname() { return ui.newname_line->text(); }

	private:
		// Ui definition of the mainwindow
		Ui::renameprompt ui;
};

#endif // PASTECOMMAND_H
