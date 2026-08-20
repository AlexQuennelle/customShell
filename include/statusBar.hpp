#pragma once

#include "compositorBackend.h"

#include <LayerShellQt/Window>
#include <QQuickView>

class StatusBar : public QQuickView
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(WindowInfo* activeWindow READ GetActiveWindow NOTIFY
				   ActiveWindowChanged)
	Q_PROPERTY(QList<Workspace*> workspaces READ GetWorkspaces NOTIFY
				   WorkspacesChanged)

	public:
	StatusBar(QWindow* parent = nullptr);

	void Init();

	auto GetActiveWindow() -> WindowInfo*;
	void SetActiveWindow(std::optional<WindowInfo&> win);
	auto GetWorkspaces() -> QList<Workspace*>;
	void SetWorkspaces(QList<Workspace*>& workspaces);

	signals:
	void ActiveWindowChanged(WindowInfo* newWin);
	void WorkspacesChanged();

	private:
	std::optional<WindowInfo&> activeWindow;
	std::optional<QList<Workspace*>&> workspaces;
};
