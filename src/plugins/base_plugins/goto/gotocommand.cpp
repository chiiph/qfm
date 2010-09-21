#include "gotocommand.h"

GotoCommand::GotoCommand() : 
	CommandBuffer(QString("goto"), Qt::Key_G)
{
	CommandBuffer::property = CommandBuffer::InPlace;
}

GotoCommand::~GotoCommand() {

}

void 
GotoCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running GotoCommand";

	QString newpath = GotoPrompt::prompt(current_dir);

	if(newpath.size() == 0) return;

	emit go(newpath);
}

GotoPrompt::GotoPrompt(QString curdir, QWidget *parent) :
	QDialog(parent) 
{
	ui.setupUi(this);

	completer = new QCompleter(this);

	QFileSystemModel *fsModel = new QFileSystemModel(completer);
	fsModel->setRootPath(curdir);
	completer->setModel(fsModel);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	completer->setMaxVisibleItems(10);

	ui.newpath_line->setCompleter(completer);
}

GotoPrompt::~GotoPrompt() {}

QString
GotoPrompt::prompt(QString path) {
	GotoPrompt g(path);
	g.set_path(path);
	g.exec();
	if(g.result() == QDialog::Accepted) {
		return g.get_newpath();
	}

	return QString("");
}

Q_EXPORT_PLUGIN2(plugin_goto, GotoCommand)
