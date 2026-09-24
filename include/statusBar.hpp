#pragma once

#include "compositorBackend.h"
#include "qUtils.h"

#include <LayerShellQt/Window>
#include <QQuickView>

class StatusBar : public QQuickView
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(WindowInfo* activeWindow READ GetActiveWindow NOTIFY
				   activeWindowChanged)
	// Q_PROPERTY(QList<Workspace*> workspaces READ GetWorkspaces NOTIFY
	// 			   workspacesChanged)
	Q_PROPERTY(QListModel* workspaces READ GetWorkspaces CONSTANT)
	Q_PROPERTY(QDateTime dateTime READ GetDateTime NOTIFY dateTimeChanged)
	Q_PROPERTY(QString clockString READ GetClockString NOTIFY dateTimeChanged)

	public:
	StatusBar(QWindow* parent = nullptr);

	void Init();

	auto GetActiveWindow() -> WindowInfo*;
	void SetActiveWindow(std::optional<WindowInfo&> win);
	// auto GetWorkspaces() -> QList<Workspace*>;
	auto GetWorkspaces() -> QListModel*;
	void SetWorkspaces(QList<Workspace*>& workspaces);
	auto GetDateTime() -> QDateTime&;
	auto GetClockString() -> QString;
	void SetDateTime(QDateTime dateTime);

	signals:
	void activeWindowChanged(WindowInfo* newWin);
	void workspacesChanged();
	void dateTimeChanged(QDateTime& newTime);

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
	QDateTime dateTime;
	std::optional<WindowInfo&> activeWindow;
	QListModel workspaces;
	// std::optional<QList<Workspace*>&> workspaces;
};
