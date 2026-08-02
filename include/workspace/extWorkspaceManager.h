#pragma once

#include "compositors/compositorBackend.h"

#include <QQmlEngine>
#include <QWaylandClientExtensionTemplate>
#include <qwayland-ext-workspace-v1.h>

class WorkspaceGroup;
class WorkspaceHandle;
class ExtWorkspaceManagerBridge;

class WLWorkspaceManager : public IWorkspaceManager
{
	Q_OBJECT

	public:
	WLWorkspaceManager();
	~WLWorkspaceManager() override;

	private:
	void OnNewWorkspaceGroup(
		struct ::ext_workspace_group_handle_v1* workspaceHandle);
	void OnNewWorkspace(struct ::ext_workspace_handle_v1* workspaceHandle);
	void OnDone();
	void OnFinished();

	friend class ExtWorkspaceManagerBridge;
	std::unique_ptr<ExtWorkspaceManagerBridge> manager{nullptr};
};

class ExtWorkspaceManagerBridge
	: public QWaylandClientExtensionTemplate<ExtWorkspaceManagerBridge>,
	  public QtWayland::ext_workspace_manager_v1
{
	public:
	explicit ExtWorkspaceManagerBridge(WLWorkspaceManager* interface) :
		QWaylandClientExtensionTemplate<ExtWorkspaceManagerBridge>(1),
		wrapper(interface)
	{ }
	~ExtWorkspaceManagerBridge() override = default;

	protected:
	void ext_workspace_manager_v1_workspace_group(
		struct ::ext_workspace_group_handle_v1* workspace_group) override;
	void ext_workspace_manager_v1_workspace(
		struct ::ext_workspace_handle_v1* workspace) override;
	void ext_workspace_manager_v1_done() override;
	void ext_workspace_manager_v1_finished() override;

	private:
	friend class WLWorkspaceManager;
	WLWorkspaceManager* wrapper;
};

class WorkspaceGroup : public QObject,
					   public QtWayland::ext_workspace_group_handle_v1
{
	Q_OBJECT
	public:
	WorkspaceGroup(struct ::ext_workspace_group_handle_v1* id)
	{
		this->init(id);
	}
	~WorkspaceGroup() override { this->destroy(); };

	protected:
	void
	ext_workspace_group_handle_v1_capabilities(uint32_t capabilities) override
	{ }
	void ext_workspace_group_handle_v1_output_enter(
		struct ::wl_output* output) override
	{ }
	void ext_workspace_group_handle_v1_output_leave(
		struct ::wl_output* output) override
	{ }
	void ext_workspace_group_handle_v1_workspace_enter(
		struct ::ext_workspace_handle_v1* workspace) override
	{ }
	void ext_workspace_group_handle_v1_workspace_leave(
		struct ::ext_workspace_handle_v1* workspace) override
	{ }
	void ext_workspace_group_handle_v1_removed() override { };
};

class WorkspaceHandle : public QObject,
						public QtWayland::ext_workspace_handle_v1
{
	Q_OBJECT; // NOLINT

	public:
	WorkspaceHandle(struct ::ext_workspace_handle_v1* id) { this->init(id); }
	~WorkspaceHandle() override { this->destroy(); }

	auto GetID() const -> QString { return this->id; }

	protected:
	void ext_workspace_handle_v1_id(const QString& id) override
	{
		this->id = id;
		qDebug() << "Workspace with ID:" << id.toStdString();
	}
	void ext_workspace_handle_v1_name(const QString& name) override
	{
		qDebug() << "New Workspace name:" << name;
	}
	void ext_workspace_handle_v1_coordinates(wl_array* coordinates) override
	{
		std::span<uint32_t> coords{static_cast<uint32_t*>(coordinates->data),
								   coordinates->size};
		QString str;
		for (auto n : coords)
		{
			str.append(std::to_string(n) + " ");
		}
		qDebug() << str;
	}
	void ext_workspace_handle_v1_state(uint32_t state) override
	{
		qDebug() << "Workspace" << this->id << "has state" << state;
	}
	void ext_workspace_handle_v1_capabilities(uint32_t capabilities) override {
	}
	void ext_workspace_handle_v1_removed() override
	{
		qDebug() << "Workspace" << this->id << "removed.";
	}

	private:
	QString id;
};
