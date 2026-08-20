#pragma once

#include "compositorBackend.h"
#include "compositors/niriBackend.h"
#include "compositors/waylandBackend.hpp"
#include "workspace/extWorkspaceManager.h"
#include "workspace/niriWorkspaceManager.h"

#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>

class ShellBackend : public QObject
{
	Q_OBJECT; // NOLINT

	public:
	ShellBackend(QObject* parent = nullptr) : QObject(parent)
	{
		this->compositor = std::make_unique<niri::NiriBackend>();
		this->connect(compositor.get(), &niri::NiriBackend::ActiveWindowChanged,
					  this, &ShellBackend::ActiveWindowChanged);
		this->workspaceManager = std::make_unique<niri::WorkspaceManager>(
			dynamic_cast<niri::NiriBackend&>(*compositor));
		this->connect(workspaceManager.get(),
					  &IWorkspaceManager::WorkspacesChanged, this,
					  &ShellBackend::WorkspacesChanged);
		// this->compositor = std::make_unique<WaylandBackend>();
		// this->workspaceManager = std::make_unique<WLWorkspaceManager>();
	};

	signals:
	void ActiveWindowChanged(const QString& output,
							 std::optional<WindowInfo&> window);
	void WorkspacesChanged(const QString& output, QList<Workspace*>& group);

	private:
	std::unique_ptr<ICompositorBackend> compositor{nullptr};
	std::unique_ptr<IWorkspaceManager> workspaceManager{nullptr};
};
