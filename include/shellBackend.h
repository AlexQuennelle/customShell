#pragma once

#include "compositorBackend.h"

#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>

class ShellBackend : public QObject
{
	Q_OBJECT; // NOLINT

	public:
	ShellBackend(QObject* parent = nullptr);

	// Workspace Manager API
	auto GetWorkspaces(const QString& outputName) -> QList<Workspace*>;
	void CreateWorkspace(const QString& name);
	void RemoveWorkspace(const QString& id);
	void SetWorkspaceOutput(const QString& id, const QString& outputName);
	void SetWorkspaceName(const QString& id, const QString& name);
	void SetWorkspaceIndex(const QString& id, uint64_t index);
	void ActivateWorkspace(const QString& id);

	signals:
	void ActiveWindowChanged(const QString& output,
							 std::optional<WindowInfo&> window);
	void WorkspacesChanged(const QString& output, QList<Workspace*>& group);

	private:
	std::unique_ptr<ICompositorBackend> compositor{nullptr};
	std::unique_ptr<IWorkspaceManager> workspaceManager{nullptr};
};
