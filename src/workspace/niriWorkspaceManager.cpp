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
				   { return val.GetWorkspaces(outputName); })
		.value_or({});
};
void WorkspaceManager::CreateWorkspace(const QString& name)
{
	this->backend->CreateWorkspace(name);
}
void WorkspaceManager::RemoveWorkspace(const QString& id)
{
	this->backend->RemoveWorkspace(id);
}
void WorkspaceManager::SetWorkspaceOutput(const QString& id,
										  const QString& outputName)
{
	this->backend->SetWorkspaceOutput(id, outputName);
}
void WorkspaceManager::SetWorkspaceName(const QString& id, const QString& name)
{
	this->backend->SetWorkspaceName(id, name);
}
void WorkspaceManager::SetWorkspaceIndex(const QString& id, uint64_t index)
{
	this->backend->SetWorkspaceIndex(id, index);
}
void WorkspaceManager::ActivateWorkspace(const QString& id)
{
	this->backend->ActivateWorkspace(id);
};

} //namespace niri
