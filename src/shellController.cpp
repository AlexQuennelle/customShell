#include "shellController.h"

#include <LayerShellQt/Window>
#include <QGuiApplication>
#include <QQuickView>
#include <memory>

ShellController::ShellController(QObject* parent) : QObject(parent)
{
	statusBar = std::make_unique<QQuickView>();
	statusBar->setResizeMode(QQuickView::ResizeMode::SizeRootObjectToView);
	statusBar->setSource(QUrl(QStringLiteral("qrc:/Shell/main.qml")));
	statusBar->setHeight(40);
	statusBar->setColor(Qt::transparent);
	statusBar->create();
	if (auto* layerWin = LayerShellQt::Window::get(statusBar.get()))
	{
		LayerShellQt::Window::Anchors barAnchors = {
			LayerShellQt::Window::AnchorTop,
			LayerShellQt::Window::AnchorLeft,
			LayerShellQt::Window::AnchorRight,
		};
		layerWin->setAnchors(barAnchors);
		layerWin->setExclusiveZone(statusBar->height());
		layerWin->setLayer(LayerShellQt::Window::LayerTop);
	}
	statusBar->show();
}
