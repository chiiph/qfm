#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <QtCore>
#include <QtPlugin>

class CommandBuffer {
	public:
		enum Property { InPlace = 0x01, Display = 0x02 };
		
		inline CommandBuffer(QString cid, Qt::Key k) : 
				command_id(cid),
				property(0),
				key(k) {}

		inline virtual ~CommandBuffer() {}

		// Sets the command_id
		inline void set_command_id(QString id) { command_id = id; }
		// Sets the propery flag
		inline void set_property(int flag) { property = flag; }
		// Adds a file path to the file list
		inline void add_file(QString file) { files << file; }
		// clears the file list
		inline void clear_buffer() { files.clear(); }

		// Returns the command_id
		inline QString get_command_id() const { return command_id; }
		// Returns the property flag
		inline int get_properties() { return property; }
		// Returns the key binded to this command
		inline Qt::Key get_key() { return key; }

		// Primitive that executes the actual command for a buffer
		virtual void run(QString current_dir, QStringList files) {}
		// Primitive that either shows some visual output
		// or returns false
		virtual bool show() { return false; }
	
	protected:
		// File list on which the command will operate
		QStringList files;
		// Command ID
		QString command_id;
		// Property flag
		int property;

		Qt::Key key;
};

Q_DECLARE_INTERFACE(CommandBuffer, "com.qfm.commandbuffer/0.1")

#endif // COMMANDBUFFER_H

