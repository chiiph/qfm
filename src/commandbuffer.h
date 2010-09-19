#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <QtCore>

class CommandBuffer : public QObject {
	Q_OBJECT

	public:
		enum Property { InPlace = 0x01, Display = 0x02 };
		
		CommandBuffer(QString cid);
		virtual ~CommandBuffer();

		// Sets the command_id
		void set_command_id(QString id);
		// Sets the propery flag
		void set_property(int flag) { property = flag; }
		// Adds a file path to the file list
		void add_file(QString file);
		// clears the file list
		void clear_buffer();

		// Returns the command_id
		QString get_command_id() const;
		// Returns the property flag
		int get_properties() { return property; }

	public slots:
		// Primitive that executes the actual command for a buffer
		virtual void run(QString current_dir, QStringList files);
		// Primitive that either shows some visual output
		// or returns false
		virtual bool show();
	
	protected:
		// File list on which the command will operate
		QStringList files;
		// Command ID
		QString command_id;
		// Property flag
		int property;
};


#endif // COMMANDBUFFER_H

