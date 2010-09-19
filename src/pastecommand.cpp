#include "pastecommand.h"

PasteCommand::PasteCommand() : 
	CommandBuffer(QString("copy")) {

}

PasteCommand::~PasteCommand() {

}

void 
PasteCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running PasteCommand";
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	QFileInfo *finfo;
	foreach(QString file, files) {
		qDebug() << file;
		finfo = new QFileInfo(file);
		
		if(finfo->fileName()=="." || finfo->fileName()=="..") continue;

		if(finfo->isFile()) {
			if(!copyFile(*finfo, dir))
				qDebug() << "ERROR: Trying to copy" << finfo->fileName() << "to" << dir.path();
		} else if(finfo->isDir()) {
			if(!copyDir(finfo->filePath(), current_dir))
				qDebug() << "ERROR: Trying to copy dir" << finfo->filePath();
		}

		delete finfo;
	}
}

bool 
PasteCommand::copyDir(const QString &src, const QString &dest) {
	qDebug() << "Copying dir" << src << dest;
	QDir dir(src);
	QDir dirdest(dest);

	if(!dir.isReadable()) return false;
	dirdest.mkdir(dir.dirName());

	QFileInfoList entries = dir.entryInfoList();

	for(QList<QFileInfo>::iterator it = entries.begin(); it!=entries.end();++it) {
		QFileInfo &finfo = *it;
		if(finfo.fileName()=="." || finfo.fileName()=="..") continue;
		if(finfo.isDir()) { copyDir(finfo.filePath(), dest+"/"+dir.dirName()); continue; }
		if(finfo.isSymLink()) { /* do something here */ continue; }
		if(finfo.isFile()) {
			if(!copyFile(finfo, QDir(dirdest.path()+"/"+dir.dirName())))
				qDebug() << "ERROR: Trying to copy" << finfo.fileName() << "to" << dirdest.path();
		} else return false;
	}
	return true;
}

bool 
PasteCommand::copyFile(QFileInfo finfo, QDir dest) {
	qDebug() << "Copying file" << finfo.fileName() << dest.absolutePath();
	if(!finfo.isReadable())
		return false;
	
	QFile file(finfo.filePath());
	file.copy(dest.absoluteFilePath(finfo.fileName()));

	return true;
}

