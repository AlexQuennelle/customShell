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
	this->connect(&backend, &ShellBackend::WorkspacesChanged, this,
				  &ShellController::OnWorkspacesChanged);
	for (auto* screen : qGuiApp->screens())
	{
		this->statusBars[screen->name()] = std::make_unique<StatusBar>();
		auto& statusBar = this->statusBars[screen->name()];
		statusBar->rootContext()->setContextProperty("bar", statusBar.get());
		statusBar->setSource(QUrl(QStringLiteral("qrc:/Shell/main.qml")));
		statusBar->Init();

		this->connect(statusBar.get(), &StatusBar::CreateWorkspaceRequested,
					  [this](const auto& name) -> void
					  { this->backend->CreateWorkspace(name); });
		this->connect(statusBar.get(), &StatusBar::RemoveWorkspaceRequested,
					  [this](const auto& id) -> void
					  { this->backend->RemoveWorkspace(id); });
		this->connect(statusBar.get(), &StatusBar::SetWorkspaceOutputRequested,
					  [this](const auto& id, const auto& outputName) -> void
					  { this->backend->SetWorkspaceOutput(id, outputName); });
		this->connect(statusBar.get(), &StatusBar::SetWorkspaceNameRequested,
					  [this](const auto& id, const auto& name) -> void
					  { this->backend->SetWorkspaceName(id, name); });
		this->connect(statusBar.get(), &StatusBar::SetWorkspaceIndexRequested,
					  [this](const auto& id, auto index) -> void
					  { this->backend->SetWorkspaceIndex(id, index); });
		this->connect(statusBar.get(), &StatusBar::ActivateWorkspaceRequested,
					  [this](const auto& id) -> void
					  { this->backend->ActivateWorkspace(id); });

		qDebug() << statusBar->screen()->name();
		statusBar->show();
	}
}
void ShellController::OnActiveWindowChanged(const QString& output,
											std::optional<WindowInfo&> window)
{
	this->statusBars[output]->SetActiveWindow(window);
}
void ShellController::OnWorkspacesChanged(const QString& output,
										  QList<Workspace*>& group)
{
	this->statusBars[output]->SetWorkspaces(group);
};
