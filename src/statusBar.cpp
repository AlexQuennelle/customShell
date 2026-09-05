#include "statusBar.hpp"

#include <qqmlcontext.h>

StatusBar::StatusBar(QWindow* parent) : QQuickView(parent)
{
	this->setResizeMode(QQuickView::ResizeMode::SizeRootObjectToView);
	this->setColor(Qt::transparent);
}
void StatusBar::Init()
{
	this->setHeight(40);
	if (auto* layerWin = LayerShellQt::Window::get(this))
	{
		LayerShellQt::Window::Anchors anchors = {
			LayerShellQt::Window::AnchorTop,
			LayerShellQt::Window::AnchorLeft,
			LayerShellQt::Window::AnchorRight,
		};
		layerWin->setKeyboardInteractivity(
			LayerShellQt::Window::KeyboardInteractivityNone);
		layerWin->setAnchors(anchors);
		layerWin->setExclusiveZone(this->height());
		layerWin->setLayer(LayerShellQt::Window::LayerTop);
	}
}
auto StatusBar::GetActiveWindow() -> WindowInfo*
{
	return this->activeWindow
		.transform([](auto& val) -> auto* { return &val; })
		.value_or(nullptr);
}
void StatusBar::SetActiveWindow(std::optional<WindowInfo&> win)
{
	this->activeWindow = win;
	emit activeWindowChanged(
		this->activeWindow
			.transform([](auto& val) -> auto* { return &val; })
			.value_or(nullptr));
}
auto StatusBar::GetWorkspaces() -> QList<Workspace*>
{
	return workspaces.value_or({});
}
void StatusBar::SetWorkspaces(QList<Workspace*>& workspaces)
{
	this->workspaces = workspaces;
	emit this->workspacesChanged();
}

void StatusBar::RequestCreateWorkspace(const QString& name)
{
	emit this->CreateWorkspaceRequested(name);
}
void StatusBar::RequestRemoveWorkspace(const QString& id)
{
	emit this->RemoveWorkspaceRequested(id);
}
void StatusBar::RequestSetWorkspaceOutput(const QString& id,
										  const QString& outputName)
{
	emit this->SetWorkspaceOutputRequested(id, outputName);
}
void StatusBar::RequestSetWorkspaceName(const QString& id, const QString& name)
{
	emit this->SetWorkspaceNameRequested(id, name);
}
void StatusBar::RequestSetWorkspaceIndex(const QString& id, uint64_t index)
{
	emit this->SetWorkspaceIndexRequested(id, index);
}
void StatusBar::RequestActivateWorkspace(const QString& id)
{
	emit this->ActivateWorkspaceRequested(id);
};
