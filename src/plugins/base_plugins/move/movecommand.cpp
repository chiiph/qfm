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
	runDelete(current_dir, files);
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
MoveCommand::copyDir(const QString &src, const QString &dest) {
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
MoveCommand::copyFile(QFileInfo finfo, QDir dest) {
	qDebug() << "Copying file" << finfo.fileName() << dest.absolutePath();
	if(!finfo.isReadable())
		return false;
	
	QFile file(finfo.filePath());
	file.copy(dest.absoluteFilePath(finfo.fileName()));

	return true;
}

void 
MoveCommand::runDelete(QString current_dir, QStringList files) {
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
}

bool 
MoveCommand::removeDir(const QString &dirName) {
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


Q_EXPORT_PLUGIN2(plugin_move, MoveCommand)
