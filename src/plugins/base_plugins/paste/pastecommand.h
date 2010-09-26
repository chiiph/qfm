#ifndef PASTECOMMAND_H
#define PASTECOMMAND_H

#include "commandbuffer.h"

class PasteCommand : public QObject, public CommandBuffer {
	Q_OBJECT
	Q_INTERFACES(CommandBuffer)
	public:
		PasteCommand();
		virtual ~PasteCommand();
	
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);

	private:
		bool copyDir(const QString &src, const QString &dest);
		bool copyFile(QFileInfo finfo, QDir dest);

		bool yesall, no;
};


#endif // PASTECOMMAND_H
