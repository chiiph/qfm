#ifndef QFMCORE_H
#define QFMCORE_H

#include <QtCore>
#include <QtPlugin>

class CommandBuffer;
class ListItem;
class Qfm;

class QfmCore : public QObject {
	Q_OBJECT

	public:
		enum Buffer {Directory, Selected, Last};

		QfmCore(Qfm *qfm);
		virtual ~QfmCore();

		// Builds and loads a buffer to the shared memory
		void load_buffer(QString command, QStringList files);
		// Executes whatever command is in the shared memory
		void flush_buffer();
		// It changes the current directory to dest if it's a dir
		// or tries to open the file
		void navigate();

		QList<ListItem *> *get_items(Buffer b) { return items[b]; }

		int get_selected_item(Buffer b) { return selected_item[b]; }
		void update_selected_item(Buffer b, int i) { selected_item[b] += i; }
		void update_dir() { filldir(); }
		void set_dir(QString dir) { directory.setPath(dir); } 
		void run(QString, QStringList);
		QString pwd() { return directory.path(); }

		QMap<Qt::Key, QString> *get_key_map() { return &key_map; }

		void select(Buffer b, int i);

	signals:
		void refresh_ui();

	public slots:
#ifdef SHAREDMEM
		// Clear the shared memory attached by detaching from it
		void clearmem();
#endif
		// Operations for plugins
		// Refreshes the displaying files
		void refresh();
		// Sets the buffer as anything it wants
		// For displaying hal resources, or simply a filtered
		// list of files
		void set_buffer(Buffer b, QList<ListItem *> files);
		// Goes to the given directory
		// it could be a relative dir, or a full path
		void gotodir(QString dir);
		// Filters the files displayed
		void set_filter(QString f);
	
	private:
		// Location of the plugins
		QDir plugins_dir;
#ifdef SHAREDMEM
		// Shared memory for IPC
		QSharedMemory shared_memory;
#endif
		// Map for a command ID to a commandbuffer object
		QMap<QString, CommandBuffer *> command_map;
		// Map for a command key to a commandbuffer ID
		QMap<Qt::Key, QString> key_map;
		// Current directory
		QDir directory;
		// Index of the current selected item
		// in the given buffer
		QList<int> selected_item;
		// List of items according to each
		// buffer in Buffer
		QList <QList<ListItem *> *> items;
		// GUI Frontend
		Qfm *qfm;
		// Timer for polling if the buffer is used or not
		QTimer timer;
		// Filter for the file list
		QRegExp filter;

		// Loads all the commands to the command_map
		void load_commands();
		// Fills directory_items with
		// the files in the current dir
		void filldir();
#ifdef SHAREDMEM
		// Writes data to shared_memory
		bool write(QString data);
		// Reads from shared_memory
		QString read();
#endif
};

#endif // QFMCORE_H
