#include "workspace/niriWorkspaceManager.h"

namespace niri
{

WorkspaceManager::WorkspaceManager() { }

auto WorkspaceManager::GetWorkspaces(QStringView outputName) -> QList<QObject*>
{
	namespace r = std::ranges;
	namespace rv = std::ranges::views;
	auto view = this->workspaces
				| rv::values
				| rv::filter([outputName](auto& val) -> bool
							 { return val.GetOutput() == outputName; })
				| rv::transform([](auto& val) -> auto* { return &val; })
				| r::to<QList<Workspace*>>();
	r::sort(view, std::less<>{}, &Workspace::GetIndex);
	return view
		   | rv::transform([](auto val) -> auto*
						   { return static_cast<QObject*>(val); })
		   | r::to<QList<QObject*>>();
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
