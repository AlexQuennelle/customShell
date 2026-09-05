#pragma once

#include "compositorBackend.h"

#include <LayerShellQt/Window>
#include <QQuickView>

class StatusBar : public QQuickView
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(WindowInfo* activeWindow READ GetActiveWindow NOTIFY
				   activeWindowChanged)
	Q_PROPERTY(QList<Workspace*> workspaces READ GetWorkspaces NOTIFY
				   workspacesChanged)

	public:
	StatusBar(QWindow* parent = nullptr);

	void Init();

	auto GetActiveWindow() -> WindowInfo*;
	void SetActiveWindow(std::optional<WindowInfo&> win);
	auto GetWorkspaces() -> QList<Workspace*>;
	void SetWorkspaces(QList<Workspace*>& workspaces);

	signals:
	void activeWindowChanged(WindowInfo* newWin);
	void workspacesChanged();

	// For C++ use only
	void CreateWorkspaceRequested(const QString& name);
	void RemoveWorkspaceRequested(const QString& id);
	void SetWorkspaceOutputRequested(const QString& id,
									 const QString& outputName);
	void SetWorkspaceNameRequested(const QString& id, const QString& name);
	void SetWorkspaceIndexRequested(const QString& id, uint64_t index);
	void ActivateWorkspaceRequested(const QString& id);

	public slots:
	void RequestCreateWorkspace(const QString& name);
	void RequestRemoveWorkspace(const QString& id);
	void RequestSetWorkspaceOutput(const QString& id,
								   const QString& outputName);
	void RequestSetWorkspaceName(const QString& id, const QString& name);
	void RequestSetWorkspaceIndex(const QString& id, uint64_t index);
	void RequestActivateWorkspace(const QString& id);

	private:
	std::optional<WindowInfo&> activeWindow;
	std::optional<QList<Workspace*>&> workspaces;
};
