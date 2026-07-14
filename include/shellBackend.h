#pragma once

#include <qobject.h>
#include <qqmlintegration.h>

class ShellBackend : QObject
{
	Q_OBJECT

	QML_ELEMENT
	QML_SINGLETON;

	public:
	ShellBackend(QObject* parent = nullptr) : QObject(parent) { };

	signals:

	private:
};
