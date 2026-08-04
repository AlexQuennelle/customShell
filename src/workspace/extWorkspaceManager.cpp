#include "workspace/extWorkspaceManager.h"

WLWorkspaceManager::WLWorkspaceManager() :
	manager(new ExtWorkspaceManagerBridge(this))
{ }
WLWorkspaceManager::~WLWorkspaceManager() { manager.reset(); }
auto WLWorkspaceManager::GetWorkspaces(QStringView outputName)
	-> QList<QObject*>
{
	return {};
}
void WLWorkspaceManager::CreateWorkspace(QStringView name) { }
void WLWorkspaceManager::RemoveWorkspace(QStringView id) { }
void WLWorkspaceManager::SetWorkspaceOutput(QStringView id,
											QStringView outputName)
{ }
void WLWorkspaceManager::SetWorkspaceName(QStringView id, QStringView name) { }
void WLWorkspaceManager::SetWorkspaceIndex(QStringView id, uint64_t index) { }
void WLWorkspaceManager::ActivateWorkspace(QStringView id) { };
;
;
;
;
;
;
void WLWorkspaceManager::OnNewWorkspaceGroup(
	struct ::ext_workspace_group_handle_v1* groupHandle)
{
	qDebug() << "New Workspace Group";
	auto test = WLWorkspaceGroup(groupHandle);
	this->groups.push_back(std::move(test));
}
void WLWorkspaceManager::OnNewWorkspace(
	struct ::ext_workspace_handle_v1* workspaceHandle)
{
	qDebug() << "New Workspace";
}
void WLWorkspaceManager::OnDone() { }
void WLWorkspaceManager::OnFinished() { }

void ExtWorkspaceManagerBridge::ext_workspace_manager_v1_workspace_group(
	struct ::ext_workspace_group_handle_v1* workspace_group)
{
	this->wrapper->OnNewWorkspaceGroup(workspace_group);
}
void ExtWorkspaceManagerBridge::ext_workspace_manager_v1_workspace(
	struct ::ext_workspace_handle_v1* workspace)
{
	this->wrapper->OnNewWorkspace(workspace);
}
void ExtWorkspaceManagerBridge::ext_workspace_manager_v1_done()
{
	this->wrapper->OnDone();
}
void ExtWorkspaceManagerBridge::ext_workspace_manager_v1_finished()
{
	this->wrapper->OnFinished();
}

WLWorkspaceGroup::WLWorkspaceGroup(struct ::ext_workspace_group_handle_v1* id) :
	manager(new ExtWorkspaceGroupBridge(this, id))
{ }
WLWorkspaceGroup::WLWorkspaceGroup(WLWorkspaceGroup&& other) noexcept
{
	this->manager.swap(other.manager);
}
WLWorkspaceGroup::~WLWorkspaceGroup() { this->manager.reset(); }
void WLWorkspaceGroup::OnCapabilities(uint32_t capabilities) { }
void WLWorkspaceGroup::OnOutputEnter(struct ::wl_output* output)
{
	qDebug() << "Entered Outpout";
}
void WLWorkspaceGroup::OnOutputLeave(struct ::wl_output* output) { }
void WLWorkspaceGroup::OnWorkspaceEnter(
	struct ::ext_workspace_handle_v1* workspace)
{ }
void WLWorkspaceGroup::OnWorkspaceLeave(
	struct ::ext_workspace_handle_v1* workspace)
{ }
void WLWorkspaceGroup::OnRemoved() { };

ExtWorkspaceGroupBridge::ExtWorkspaceGroupBridge(
	WLWorkspaceGroup* interface, struct ::ext_workspace_group_handle_v1* id) :
	QWaylandClientExtensionTemplate<ExtWorkspaceGroupBridge>(1),
	wrapper(interface)
{
	this->init(id);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_capabilities(
	uint32_t capabilities)
{
	this->wrapper->OnCapabilities(capabilities);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_output_enter(
	struct ::wl_output* output)
{
	this->wrapper->OnOutputEnter(output);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_output_leave(
	struct ::wl_output* output)
{
	this->wrapper->OnOutputLeave(output);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_workspace_enter(
	struct ::ext_workspace_handle_v1* workspace)
{
	this->wrapper->OnWorkspaceEnter(workspace);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_workspace_leave(
	struct ::ext_workspace_handle_v1* workspace)
{
	this->wrapper->OnWorkspaceLeave(workspace);
}
void ExtWorkspaceGroupBridge::ext_workspace_group_handle_v1_removed()
{
	this->wrapper->OnRemoved();
};
