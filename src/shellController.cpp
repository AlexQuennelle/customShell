#include "shellController.h"

#include <LayerShellQt/Window>
#include <QGuiApplication>
#include <QQuickView>
#include <memory>
#include <qqmlcontext.h>

ShellController::ShellController(ShellBackend& backend, QObject* parent) :
	QObject(parent), backend(backend)
{
	this->connect(&backend, &ShellBackend::ActiveWindowChanged, this,
				  &ShellController::OnActiveWindowChanged);
	for (auto* screen : qGuiApp->screens())
	{
		this->statusBars[screen->name()] = std::make_unique<StatusBar>();
		auto& statusBar = this->statusBars[screen->name()];
		statusBar->rootContext()->setContextProperty("bar", statusBar.get());
		statusBar->setSource(QUrl(QStringLiteral("qrc:/Shell/main.qml")));
		statusBar->Init();
		qDebug() << statusBar->screen()->name();
		statusBar->show();
	}
}
void ShellController::OnActiveWindowChanged(const QString& output,
											std::optional<WindowInfo&> window)
{
	this->statusBars[output]->SetActiveWindow(window);
}
