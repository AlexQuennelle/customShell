#pragma once

#include <QQmlEngine>
#include <QWaylandClientExtensionTemplate>
#include <print>
#include <qwayland-ext-workspace-v1.h>

class WorkspaceGroup;
class WorkspaceHandle;

class WorkspaceManager
	: public QWaylandClientExtensionTemplate<WorkspaceManager>,
	  public QtWayland::ext_workspace_manager_v1
{
	Q_OBJECT
	// QML_ELEMENT
	public:
	explicit WorkspaceManager(QObject* parent = nullptr) :
		QWaylandClientExtensionTemplate<WorkspaceManager>(1)
	{ }
	~WorkspaceManager() override = default;

	protected:
	void ext_workspace_manager_v1_workspace_group(
		struct ::ext_workspace_group_handle_v1* workspace_group) override
	{ }
	void ext_workspace_manager_v1_workspace(
		struct ::ext_workspace_handle_v1* workspace) override
	{ }
	void ext_workspace_manager_v1_done() override { }
	void ext_workspace_manager_v1_finished() override { }
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
	Q_OBJECT

	public:
	WorkspaceHandle(struct ::ext_workspace_handle_v1* id) { this->init(id); }
	~WorkspaceHandle() override { this->destroy(); }

	protected:
	void ext_workspace_handle_v1_id(const QString& id) override
	{
		std::print("Workspace ID: {}", id.toStdString());
	}
	void ext_workspace_handle_v1_name(const QString& name) override { }
	void ext_workspace_handle_v1_coordinates(wl_array* coordinates) override { }
	void ext_workspace_handle_v1_state(uint32_t state) override { }
	void ext_workspace_handle_v1_capabilities(uint32_t capabilities) override {
	}
	void ext_workspace_handle_v1_removed() override { }
};
