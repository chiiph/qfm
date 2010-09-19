#include "deletecommand.h"

DeleteCommand::DeleteCommand() : 
	CommandBuffer(QString("delete")) 
{
	property = CommandBuffer::InPlace | CommandBuffer::Display;
	doit = false;
}

DeleteCommand::~DeleteCommand() {

}

void 
DeleteCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running DeleteCommand";
	if(doit) {
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
DeleteCommand::show() {
	int res = QMessageBox::question(0, tr("Are you?"), tr("Are you sure you want to delete this file(s)?"), QMessageBox::Yes | QMessageBox::No);

	doit = (res == QMessageBox::Yes);
	return true;
}

bool 
DeleteCommand::removeDir(const QString &dirName) {
	bool result = true;
	QDir dir(dirName);

	if (dir.exists(dirName)) {
		foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
			if (info.isDir())
				result = removeDir(info.absoluteFilePath());
			else
				result = QFile::remove(info.absoluteFilePath());

			if (!result)
				return result;
		}
	
		result = dir.rmdir(dirName);
	}

	return result;
}
