#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <QtCore>
#include <QtPlugin>

class Navigation {
	public:
		inline Navigation(QString nid, Qt::Key k) : 
				navigation_id(nid),
				key(k) {}
		inline virtual ~Navigation() {}

		// Sets the navigation_id
		inline void set_navigation_id(QString id) { navigation_id = id; }

		// Returns the command_id
		inline QString get_command_id() const { return command_id; }
		inline QList<QFileInfo *> *navigate(QString dir) 
			{ return 0; }
		inline QString get(int i) { return QString(); }
	
	protected:
		// Navigation ID
		QString navigation_id;
		// Current dir
		QString current_dir;
		// Files in the current dir
		QList<QFileInfo> *files;
};

Q_DECLARE_INTERFACE(Navigation, "com.qfm.navigation/0.1")

#endif // NAVIGATION_H

