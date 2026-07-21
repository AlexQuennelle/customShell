#include "workspace/extWorkspaceManager.h"

WLWorkspaceManager::WLWorkspaceManager() :
	manager(new ExtWorkspaceManagerBridge(this))
{ }
WLWorkspaceManager::~WLWorkspaceManager() { manager.reset(); }
void WLWorkspaceManager::OnNewWorkspaceGroup(
	struct ::ext_workspace_group_handle_v1* workspaceHandle)
{
	qDebug() << "New Workspace Group";
}
void WLWorkspaceManager::OnNewWorkspace(
	struct ::ext_workspace_handle_v1* workspaceHandle)
{ }
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
