#include <LayerShellQt/Window>
#include <QGuiApplication>
#include <QQuickView>

auto main(int argc, char* argv[]) -> int
{
	qputenv("QT_QPA_PLATFORM", "wayland");

	QGuiApplication app(argc, argv);

	QQuickView view;

	view.setResizeMode(QQuickView::ResizeMode::SizeRootObjectToView);
	view.setSource(QUrl(QStringLiteral("qrc:/Shell/main.qml")));
	view.setHeight(40);
	view.setColor(Qt::transparent);
	view.create();
	if (auto* layerWin = LayerShellQt::Window::get(&view))
	{
		LayerShellQt::Window::Anchors barAnchors = {
			LayerShellQt::Window::AnchorTop,
			LayerShellQt::Window::AnchorLeft,
			LayerShellQt::Window::AnchorRight,
		};
		layerWin->setAnchors(barAnchors);
		layerWin->setExclusiveZone(view.height());
		layerWin->setLayer(LayerShellQt::Window::LayerTop);
	}
	view.show();
	return app.exec();
}
