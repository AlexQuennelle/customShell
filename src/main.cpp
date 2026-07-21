#include "shellBackend.h"
#include "shellController.h"

#include <LayerShellQt/Window>
#include <QGuiApplication>
#include <QQuickView>

auto main(int argc, char* argv[]) -> int
{
	qputenv("QT_QPA_PLATFORM", "wayland");
	qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");

	QGuiApplication app(argc, argv);

	// QQuickView view;

	ShellBackend& backend = ShellBackend::Instance();
	qmlRegisterSingletonInstance("Shell.ShellBackend", 1, 0, "Backend",
								 &backend);

	ShellController controller;

	return app.exec();
}
