#include "workspace/niriWorkspaceManager.h"

namespace niri
{

WorkspaceManager::WorkspaceManager(niri::NiriBackend& backend) :
	backend(backend)
{
	this->connect(&*this->backend, &niri::NiriBackend::WorkspacesChanged, this,
				  &IWorkspaceManager::WorkspacesChanged);
}

auto WorkspaceManager::GetWorkspaces(const QString& outputName)
	-> QList<Workspace*>
{
	return this->backend
		.transform([&outputName](auto& val) -> QList<Workspace*>
				   { return val.Workspaces(outputName); })
		.value_or({});
};
void WorkspaceManager::CreateWorkspace(const QString& name) { }
void WorkspaceManager::RemoveWorkspace(const QString& id) { }
void WorkspaceManager::SetWorkspaceOutput(const QString& id,
										  const QString& outputName)
{ }
void WorkspaceManager::SetWorkspaceName(const QString& id, const QString& name)
{ }
void WorkspaceManager::SetWorkspaceIndex(const QString& id, uint64_t index) { }
void WorkspaceManager::ActivateWorkspace(const QString& id) { };

} //namespace niri
