#include "shellBackend.h"
#include "shellController.h"

#include <LayerShellQt/Window>
#include <QGuiApplication>
#include <QQuickView>
#include <glaze/core/feature_test.hpp>

auto main(int argc, char* argv[]) -> int
{
	qputenv("QT_QPA_PLATFORM", "wayland");
	qputenv("QT_ASSUME_STDERR_HAS_CONSOLE", "1");

	QGuiApplication app(argc, argv);

	ShellBackend backend;
	ShellController controller{backend};

	return app.exec();
}
