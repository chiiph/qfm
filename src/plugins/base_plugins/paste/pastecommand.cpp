#include "pastecommand.h"
#include <QtGui>

PasteCommand::PasteCommand() : 
	CommandBuffer(QString("copy"), Qt::Key_Y)
{
	yesall = false;
	no = false;
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

		if(no)
			break;
	}
	no = false;
	yesall = false;
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
	QFile filedest(dest.absoluteFilePath(finfo.fileName()));

	if(filedest.exists() and !yesall) {
		int res = QMessageBox::question(0, tr("Overwrite"),
				tr("Do you want to overwite ")+finfo.fileName()+tr("?"),
				QMessageBox::Yes | QMessageBox::YesToAll | QMessageBox::No | QMessageBox::Cancel,
				QMessageBox::Yes);
		if(res == QMessageBox::Cancel) {
			no = true;
			return true;
		}
		if(res != QMessageBox::Yes and res != QMessageBox::YesToAll)
			return false;
		if(res == QMessageBox::YesToAll)
			yesall = true;
	}

	if(filedest.exists())
		filedest.remove();

	return file.copy(dest.absoluteFilePath(finfo.fileName()));
}

Q_EXPORT_PLUGIN2(plugin_paste, PasteCommand)
