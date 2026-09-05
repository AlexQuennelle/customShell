#include "shellBackend.h"
#include "compositors/niriBackend.h"
#include "compositors/waylandBackend.hpp"
#include "workspace/extWorkspaceManager.h"
#include "workspace/niriWorkspaceManager.h"

ShellBackend::ShellBackend(QObject* parent) : QObject(parent)
{
	this->compositor = std::make_unique<niri::NiriBackend>();
	this->connect(compositor.get(), &niri::NiriBackend::ActiveWindowChanged,
				  this, &ShellBackend::ActiveWindowChanged);
	this->workspaceManager = std::make_unique<niri::WorkspaceManager>(
		dynamic_cast<niri::NiriBackend&>(*compositor));
	this->connect(workspaceManager.get(), &IWorkspaceManager::WorkspacesChanged,
				  this, &ShellBackend::WorkspacesChanged);
	// this->compositor = std::make_unique<WaylandBackend>();
	// this->workspaceManager = std::make_unique<WLWorkspaceManager>();
}

auto ShellBackend::GetWorkspaces(const QString& outputName) -> QList<Workspace*>
{
	return workspaceManager->GetWorkspaces(outputName);
}
void ShellBackend::CreateWorkspace(const QString& name)
{
	workspaceManager->CreateWorkspace(name);
}
void ShellBackend::RemoveWorkspace(const QString& id)
{
	workspaceManager->RemoveWorkspace(id);
}
void ShellBackend::SetWorkspaceOutput(const QString& id,
									  const QString& outputName)
{
	workspaceManager->SetWorkspaceOutput(id, outputName);
}
void ShellBackend::SetWorkspaceName(const QString& id, const QString& name)
{
	workspaceManager->SetWorkspaceName(id, name);
}
void ShellBackend::SetWorkspaceIndex(const QString& id, uint64_t index)
{
	workspaceManager->SetWorkspaceIndex(id, index);
}
void ShellBackend::ActivateWorkspace(const QString& id)
{
	workspaceManager->ActivateWorkspace(id);
};
