#include "workspace/niriWorkspaceManager.h"

namespace niri
{

WorkspaceManager::WorkspaceManager() { }

auto WorkspaceManager::GetWorkspaces(QStringView outputName) -> QList<QObject*>
{
	return {};
};
void WorkspaceManager::CreateWorkspace(QStringView name) { }
void WorkspaceManager::RemoveWorkspace(QStringView id) { }
void WorkspaceManager::SetWorkspaceOutput(QStringView id,
										  QStringView outputName)
{ }
void WorkspaceManager::SetWorkspaceName(QStringView id, QStringView name) { }
void WorkspaceManager::SetWorkspaceIndex(QStringView id, uint64_t index) { }
void WorkspaceManager::ActivateWorkspace(QStringView id) { };

} //namespace niri
