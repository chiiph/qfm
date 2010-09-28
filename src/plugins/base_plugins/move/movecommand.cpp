#include "movecommand.h"

MoveCommand::MoveCommand() : 
	CommandBuffer(QString("move"), Qt::Key_C) 
{
	
}

MoveCommand::~MoveCommand() {

}

void 
MoveCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running MoveCommand";
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	runPaste(current_dir, files);
}

void 
MoveCommand::runPaste(QString current_dir, QStringList files) {
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	QFileInfo *finfo;
	foreach(QString file, files) {
		qDebug() << file;
		finfo = new QFileInfo(file);
		
		if(finfo->fileName()=="." || finfo->fileName()=="..") continue;

		if(finfo->isFile()) {
			if(!QFile::rename(finfo->filePath(), dir.path()+"/"+finfo->fileName()))
				qDebug() << "ERROR: Trying to move" << finfo->fileName() << "to" << dir.path();
		} else if(finfo->isDir()) {
			if(!dir.rename(finfo->filePath(), current_dir+"/"+finfo->fileName()))
				qDebug() << "ERROR: Trying to move dir" << finfo->filePath();
		}

		delete finfo;
	}
}

Q_EXPORT_PLUGIN2(plugin_move, MoveCommand)
