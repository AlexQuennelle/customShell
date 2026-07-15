#pragma once

#include <qobject.h>
#include <qquickview.h>

class ShellController : public QObject
{
	Q_OBJECT
	public:
	explicit ShellController(QObject* parent = nullptr);

	private:
	std::unique_ptr<QQuickView> statusBar{nullptr};
};
