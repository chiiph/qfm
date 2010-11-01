#include "qfmcore.h"
#include "../qfm.h"
#include "../interfaces/commandbuffer.h"
#include "listitem.h"

QfmCore::QfmCore(Qfm *q) : 
	QObject(),
#ifdef SHAREDMEM
	shared_memory("qfm_mem"),
#endif
	directory(QDir::home()),
	qfm(q) {
	load_commands();

#ifdef SHAREDMEM
	// "After crash" shared memory handler
	shared_memory.attach();
	if(shared_memory.isAttached())
		shared_memory.detach();
#endif

	connect(&timer, SIGNAL(timeout()), this, SLOT(clearmem()));

	// Init buffer items
	for(int i = 0; i < QfmCore::Last; i++)
		items.append(new QList<ListItem *>);

	// Init buffer selection
	for(int i = 0; i < QfmCore::Last; i++)
		selected_item.append(0);

	filter.setPattern("");
	filter.setCaseSensitivity(Qt::CaseInsensitive);

	filldir();
	QSettings::setDefaultFormat(QSettings::IniFormat);
}

QfmCore::~QfmCore() {
#ifdef SHAREDMEM
	if(shared_memory.isAttached()) {
		qDebug() << "INFO: Detaching from shared_memory.";
		shared_memory.detach();
	}
#endif
}

void 
QfmCore::filldir() {
	for(int i = 0; i < QfmCore::Last; i++) {
		while(!get_items((Buffer)i)->empty())
			delete (get_items((Buffer)i))->takeFirst();
		selected_item[i] = -1;
	}

	foreach(QString file, directory.entryList(QDir::NoFilter, QDir::DirsFirst | QDir::Name).filter(filter)) {
		if(file == "." or file == "..") continue;
		*(get_items(QfmCore::Directory)) << new ListItem(file, directory.absolutePath()+"/"+file);
	}

	for(int i = 0; i < QfmCore::Last; i++) {
		if(!get_items((Buffer)i)->empty()) {
//            get_items((Buffer)i)->at(0)->toggle_selected();
			selected_item[i] = -1;
		}
	}
}

void 
QfmCore::load_buffer(QString command, QStringList files) {
	qDebug() << command;
	qDebug() << files;

	QString strbuf = "Qfm;";
	strbuf += command + ";";
	QString concatbuf = "";

	foreach(QString file, files) {
		strbuf += file + ";";
		concatbuf += file + ";";
	}

#ifdef SHAREDMEM
	write(strbuf);

	timer.start(10);
#else
	qDebug() << "STARTINGGG";
	QClipboard *clipboard = QApplication::clipboard();
	if(!clipboard->text().startsWith("Qfm"))
		clipboard->setText("");
	QStringList text = clipboard->text().split(";", QString::SkipEmptyParts);
	qDebug() << text;
	qDebug() << text.count();
	if(text.count() < 3 and text.count() != 0) return;
	qDebug() << "PASO";
	if(text.count() != 0) {
		if(text.takeAt(0) == "Qfm") {
			qDebug() << "it's a qfm buffer!!";
			if(text.takeAt(0) == command) {
				qDebug() << "concatening!!!";
				// Use a set to avoid having repeated files/dirs
				QSet<QString> set;
				QString buffer = clipboard->text() + concatbuf;
				QStringList listbuffer = buffer.split(";", QString::SkipEmptyParts);
				listbuffer.takeAt(0);
				listbuffer.takeAt(0);
				set = QSet<QString>::fromList(listbuffer);
				QString newbuffer = "Qfm;";
				newbuffer += command+";";
				QStringList newl = set.toList();
				newbuffer += newl.join(";");
				qDebug() << newbuffer;
				clipboard->setText(newbuffer);
				return;
			}
		}
	}
	qDebug() << "not concatening";
	clipboard->setText(strbuf);	
#endif
}

void 
QfmCore::flush_buffer() {
	qDebug() << "flushing";
	QString current_dir = directory.absolutePath();
	QString text = "";

#ifdef SHAREDMEM
	text = read();
	text = text.replace(0,1,"0");
	qDebug() << "TEXT NOW>" << text;
	write(text);
	qDebug() << "A VER" << read();
#else
	QClipboard *clipboard = QApplication::clipboard();
	text = clipboard->text();
	clipboard->setText("");
#endif

	QStringList files = text.split(";", QString::SkipEmptyParts);
	QString pid = files.takeAt(0);
	qDebug() << text;
	if(pid != "Qfm") return; // Check whether the buffer was set by qfm
	QString cmd = files.takeAt(0);
	qDebug() << "Command to run:";
	qDebug() << cmd;
	qDebug() << files;
	command_map[cmd]->run(current_dir, files);
}

void 
QfmCore::load_commands() {
	plugins_dir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (plugins_dir.dirName().toLower() == "debug" || plugins_dir.dirName().toLower() == "release")
		plugins_dir.cdUp();
#elif defined(Q_OS_MAC)
	if (plugins_dir.dirName() == "MacOS") {
		plugins_dir.cdUp();
		plugins_dir.cdUp();
		plugins_dir.cdUp();
	}
#endif
	plugins_dir.cd("plugins");

	CommandBuffer *cb = NULL;
	QObject *plugin = NULL;

	foreach (QString fileName, plugins_dir.entryList(QDir::Files)) {
		qDebug() << plugins_dir.absoluteFilePath(fileName);
		QPluginLoader loader(plugins_dir.absoluteFilePath(fileName));
		if(!loader.load())
			qDebug() << loader.errorString();
		plugin = loader.instance();
		qDebug() << plugin;
		if (plugin) {
			cb = qobject_cast<CommandBuffer *>(plugin);
			this->connect(plugin, SIGNAL(go(QString)), SLOT(gotodir(QString)));
			command_map[cb->get_command_id()] = cb;
			key_map[cb->get_key()] = cb->get_command_id();
			qDebug() << "Loading" << cb->get_command_id();
		}
	}
}

void 
QfmCore::navigate() {
	if(selected_item.at(QfmCore::Directory) < 0)
		return;

	QFileInfo finfo(get_items(QfmCore::Directory)->at(selected_item.at(QfmCore::Directory))->get_full_path());
	if(finfo.isDir()) {
		directory.cd(finfo.filePath());
		set_filter("");
		filldir();
		selected_item[QfmCore::Directory] = -1;
	} else {
		QString ext = finfo.suffix();
		QSettings settings("qfm", "rc");
		QString app = settings.value(ext, "").toString();
		qDebug() << "To Open" << ext << app;
		if(app.size() == 0) {
			QMessageBox::warning(0, tr("Unknown file extension"),
					tr("Don't know what to do with this. Edit the file preferences"));
			return;
		}
		QStringList args;
		args << finfo.filePath();
		QProcess *proc = new QProcess(this);
		proc->start(app, args);
	}
}

#ifdef SHAREDMEM
void 
QfmCore::clearmem() {
	if(shared_memory.lock()) {
		QBuffer buffer;
		QDataStream in(&buffer);
		QString text;

		buffer.setData((char *)shared_memory.data(), shared_memory.size());
		buffer.open(QBuffer::ReadWrite);

		in >> text;
		if(text.startsWith("0")) {
			qDebug() << "INFO: Cleaning shared memory";
			shared_memory.detach();
			timer.stop();
		}
		shared_memory.unlock();
	}
}

bool
QfmCore::write(QString data) {
	QByteArray ba;
	QBuffer buffer(&ba);

	if(shared_memory.isAttached())
		shared_memory.detach();

	buffer.open(QBuffer::ReadWrite);
	QDataStream out(&buffer);

	out << data;

	int size = buffer.size();

	if(!shared_memory.create(size)) {
		if(!shared_memory.attach()) {
			qDebug() << "ERROR: Can't create shared_memory.";
			return false;
		}
	}

	shared_memory.lock();
	char *to = (char *)shared_memory.data();
	const char *from = buffer.data().data();
	memcpy(to, from, qMin(shared_memory.size(), size));
	shared_memory.unlock();

	return true;
}

QString
QfmCore::read() {
	if(!shared_memory.attach() and !shared_memory.isAttached()) {
		qDebug() << "ERROR: Can't attach to shared_memory.";
		return QString();
	}

	if(shared_memory.size() == 0) {
		qDebug() << "INFO: There's nothing in the buffer to do.";
		return QString();
	}

	QBuffer buffer;
	QDataStream in(&buffer);
	QString text;

	shared_memory.lock();
	buffer.setData((char *)shared_memory.data(), shared_memory.size());
	buffer.open(QBuffer::ReadWrite);

	in >> text;
	shared_memory.unlock();

	shared_memory.detach();

	return text;
}
#endif

void
QfmCore::run(QString cmd, QStringList files) {
	if(!command_map[cmd]->get_properties()) {
		load_buffer(cmd, files);
		return;
	}
	
	if(command_map[cmd]->get_properties() & CommandBuffer::Display) {
		qDebug() << "Display command";
		command_map[cmd]->show();
	}

	if(command_map[cmd]->get_properties() & CommandBuffer::InPlace) {
		qDebug() << "InPlace command";
		QString current_dir = directory.absolutePath();
		command_map[cmd]->run(current_dir, files);
	}
}

void
QfmCore::refresh() {
	filldir();
	emit refresh_ui();
}

void
QfmCore::set_buffer(Buffer b, QList<ListItem *> files) {
	while(!get_items(b)->empty())
		delete (get_items(b))->takeFirst();

	foreach(ListItem *file, files) {
		*(get_items(b)) << file;
	}

	if(!get_items(b)->empty())
		get_items(b)->at(0)->toggle_selected();
	else
		selected_item[b] = -1;

	emit refresh_ui();
}

void
QfmCore::gotodir(QString dir) {
	qDebug() << "gotoing!!";
	directory.cd(dir);
	filldir();
	emit refresh_ui();
}

void 
QfmCore::set_filter(QString f) {
	filter = QRegExp(f, Qt::CaseInsensitive);
}

void 
QfmCore::select(Buffer b, int i) {
	int prev = selected_item[b];
	get_items(b)->at(prev)->toggle_selected();
	get_items(b)->at(i)->toggle_selected();
	selected_item[b] = i;
}
