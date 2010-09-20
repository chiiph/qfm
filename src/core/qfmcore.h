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
		QList<ListItem *> *get_selected_items() { return &selected_items; }
		QList<ListItem *> *get_directory_items() { return &directory_items; }

		int get_selected_item(Buffer b) { return selected_item[b]; }
		void update_selected_item(Buffer b, int i) { selected_item[b] += i; }
		void update_dir() { filldir(); }
		void set_dir(QString dir) { directory.setPath(dir); } 
		void run(QString, QStringList);

		QMap<Qt::Key, QString> *get_key_map() { return &key_map; }

	public slots:
		// Clear the shared memory attached by detaching from it
		void clearmem();
	
	private:
		// Location of the plugins
		QDir plugins_dir;
		// Shared memory for IPC
		QSharedMemory shared_memory;
		// Map for a command ID to a commandbuffer object
		QMap<QString, CommandBuffer *> command_map;
		// Map for a command key to a commandbuffer ID
		QMap<Qt::Key, QString> key_map;
		// Current directory
		QDir directory;
		// Index of the current selected item
		// in the given buffer
		QList<int> selected_item;
		// List of selected ListItems
		QList <ListItem *> selected_items;
		// List of directory ListItems
		QList <ListItem *> directory_items;
		// List of items according to each
		// buffer in Buffer
		QList <QList<ListItem *> *> items;
		// GUI Frontend
		Qfm *qfm;
		// Timer for polling if the buffer is used or not
		QTimer timer;

		// Loads all the commands to the command_map
		// TODO: make this more generic
		void load_commands();
		// Fills directory_items with
		// the files in the current dir
		void filldir();
		// Writes data to shared_memory
		bool write(QString data);
		// Reads from shared_memory
		QString read();
};

#endif // QFMCORE_H
