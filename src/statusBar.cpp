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
	emit ActiveWindowChanged(
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
	emit this->WorkspacesChanged();
}
