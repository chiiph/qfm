#include "renamecommand.h"

RenameCommand::RenameCommand() : 
	CommandBuffer(QString("delete")) 
{
	property = CommandBuffer::InPlace | CommandBuffer::Display;
	newname = "";
}

RenameCommand::~RenameCommand() {

}

void 
RenameCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running RenameCommand";
	if(newname.size() != 0) {
		QDir dir(current_dir);
		
		if(!dir.isReadable()) return;

		QFileInfo *finfo;
		foreach(QString file, files) {
			qDebug() << file;
			finfo = new QFileInfo(file);
			
			if(finfo->fileName()=="." || finfo->fileName()=="..") continue;

			if(finfo->isFile()) {
				qDebug() << "Removing file";
				if(!QFile::remove(finfo->filePath()))
					qDebug() << "ERROR: Trying to delete" << finfo->fileName() << "to" << dir.path();
			} else if(finfo->isDir()) {
				if(!removeDir(finfo->filePath()))
					qDebug() << "ERROR: Trying to copy dir" << finfo->filePath();
			}

			delete finfo;
		}
	} else
		qDebug() << "Not doing it";
}

bool
RenameCommand::show() {
	int res = QMessageBox::question(0, tr("Are you?"), tr("Are you sure you want to delete this file(s)?"), QMessageBox::Yes | QMessageBox::No);

	doit = (res == QMessageBox::Yes);
	return true;
}

RenamePrompt::RenamePrompt(QWidget *parent) : 
	QDialog(parent)
{
	ui.setupUi(this);
}

RenamePrompt::~RenamePrompt() {

}

QString
RenamePrompt::prompt() {
	RenamePrompt r;
	r.exec()
}
