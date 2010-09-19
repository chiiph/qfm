#ifndef PASTECOMMAND_H
#define PASTECOMMAND_H

#include "commandbuffer.h"

class PasteCommand : public CommandBuffer {
	public:
		PasteCommand();
		virtual ~PasteCommand();
	
	public slots:
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);

	private:
		bool copyDir(const QString &src, const QString &dest);
		bool copyFile(QFileInfo finfo, QDir dest);
};


#endif // PASTECOMMAND_H
