#include "compositors/niriBackend.h"
#include "compositorBackend.h"
#include "compositors/niriTypes.hpp"

#include <qdebug.h>

namespace niri
{

NiriBackend::NiriBackend() : eventSock(this), cmdSock(this)
{
	QString sockAddr = std::getenv("NIRI_SOCKET");

	connect(&eventSock, &QLocalSocket::connected,
			[this, &sockAddr]() -> void
			{
				qDebug() << "Connecting to" << sockAddr;
				if (eventSock.write("\"EventStream\"\n") == -1)
				{
					qFatal() << "ERROR: failed to connect to Niri event stream";
				}
			});
	connect(&eventSock, &QLocalSocket::readyRead, this,
			&NiriBackend::ReadMessages);
	eventSock.connectToServer(sockAddr);

	cmdSock.connectToServer(sockAddr);
}

auto NiriBackend::GetWorkspaces(const QString& outputName) -> QList<Workspace*>
{
	return this->workspaceGroups[outputName];
}
void NiriBackend::CreateWorkspace(const QString& name) { }
void NiriBackend::RemoveWorkspace(const QString& id) { }
void NiriBackend::SetWorkspaceOutput(const QString& id,
									 const QString& outputName)
{ }
void NiriBackend::SetWorkspaceName(const QString& id, const QString& name) { }
void NiriBackend::SetWorkspaceIndex(const QString& id, uint64_t index) { }
void NiriBackend::ActivateWorkspace(const QString& id)
{
	// TODO: Create action type to serialize to json with glaze
	std::string cmd = R"({"Action":{"FocusWorkspace":{"reference":{"Id":)"
					  + id.toStdString()
					  + "}}}}\n";
	this->cmdSock.write(cmd.c_str());
}

void NiriBackend::ReadMessages()
{
	QByteArray line;
	while (this->eventSock.canReadLine())
	{
		line = this->eventSock.readLine();
		ProcessMessage(line);
	}
}
void NiriBackend::ProcessMessage(const std::string_view message) // NOLINT
{
	niri::Response response;
	auto err = glz::read<glzOpts{}>(response, message);
	if (err.ec == glz::error_code::none)
	{
		auto out = glz::write<glzOpts{}>(response).value_or("ERROR");
		qDebug().noquote() << out;
	}
	niri::Event event;
	err = glz::read<glzOpts{}>(event, message);
	if (err.ec == glz::error_code::none)
	{
		auto eventSwitch = Overload{
			[this](WorkspacesChangedEvent& event) -> void
			{
				workspaceGroups.clear();
				for (auto& data : event.workspaces)
				{
					const QString nameStr
						= QString::fromStdString(data.name.value_or(""));
					const QString output
						= QString::fromStdString(data.output.value_or(""));

					auto [it, success] = this->workspaces.try_emplace(
						data.id, QString::number(data.id), nameStr, output,
						data.active_window_id, data.idx, data.is_urgent,
						data.is_active, data.is_focused);
					auto& [_, workspace] = *it;

					if (!success)
					{
						workspace.SetName(nameStr);
						workspace.SetOutput(output);
						workspace.SetIndex(data.idx);
						workspace.SetActive(data.is_active);
						workspace.SetActiveWindowId(data.active_window_id);
					}
					if (data.is_active || data.is_focused)
					{
						this->activeWorkspaces[workspace.GetOutput()] = data.id;
						auto window = workspace.GetActiveWindowID().transform(
							[this](auto id) -> auto&
							{ return this->windows[id]; });
						ChangeActiveWindow(workspace, window);
					}
					workspace.SetFocused(data.is_focused);
					workspaceGroups[output].push_back(&workspace);
				}
				namespace rv = std::views;
				auto openWorkspaces
					= event.workspaces
					  | rv::transform([](auto& value) -> uint64_t
									  { return value.id; })
					  | std::ranges::to<std::unordered_set<uint64_t>>();
				auto pred = [&openWorkspaces](const auto& pair) -> bool
				{
					return !openWorkspaces.contains(pair.first);
				};
				for (auto& [_, val] : this->workspaces | rv::filter(pred))
				{
					val.SetDead();
				}
				for (const auto& [key, val] : workspaceGroups.asKeyValueRange())
				{
					std::ranges::sort(val, {}, &Workspace::GetIndex);
					emit this->WorkspacesChanged(key, val);
				}
				std::erase_if(this->workspaces, [](auto& pair) -> bool
							  { return pair.second.GetDead(); });
			},
			[this](WorkspaceUrgencyChangedEvent& event) -> void
			{ this->workspaces.at(event.id).SetUrgent(event.urgent); },
			[this](WorkspaceActivatedEvent& event) -> void
			{
				auto& workspace = this->workspaces.at(event.id);
				this->workspaces.at(activeWorkspaces[workspace.GetOutput()])
					.SetActive(false);
				this->activeWorkspaces[workspace.GetOutput()] = event.id;
				workspace.SetActive(true);
				auto window = workspace.GetActiveWindowID().transform(
					[this](auto id) -> auto& { return this->windows[id]; });
				ChangeActiveWindow(workspace, window);
				if (event.focused)
				{
					this->workspaces.at(this->focusedWorkspaceID)
						.SetFocused(false);
					workspace.SetFocused(true);
					this->focusedWorkspaceID = event.id;
				}
			},
			[this](WorkspaceActiveWindowChangedEvent& event) -> void
			{
				auto& workspace = this->workspaces.at(event.workspace_id);
				auto window = event.active_window_id.transform(
					[this](auto& id) -> auto& { return this->windows[id]; });
				ChangeActiveWindow(workspace, window);
			},
			[this](WindowsChangedEvent& event) -> void
			{
				for (auto& window : event.windows)
				{
					if (this->windows.contains(window.id))
					{
						auto workspaceID
							= this->windows.at(window.id).GetWorkspace();
						if (workspaceID.has_value())
						{
							this->workspaces.at(workspaceID.value())
								.RemoveWindow(window.id);
						}
					}
					this->windows[window.id] = WindowInfo(
						QString::fromStdString(window.title.value_or("")),
						QString::fromStdString(window.app_id.value_or("")),
						window.workspace_id);
					if (window.workspace_id.has_value())
					{
						auto& workspace
							= this->workspaces.at(window.workspace_id.value());
						workspace.AddWindow(window.id);
						if (window.is_focused)
						{
							workspace.SetActiveWindowId(window.id);
							if (this->workspaces.contains(focusedWorkspaceID))
							{
								this->workspaces.at(this->focusedWorkspaceID)
									.SetFocused(false);
							}
							workspace.SetFocused(true);
							this->focusedWorkspaceID
								= window.workspace_id.value();
						}
						ChangeActiveWindow(workspace, this->windows[window.id]);
					}
				}
			},
			[this](WindowOpenedOrChangedEvent& event) -> void
			{
				auto& window{event.window};
				if (this->windows.contains(window.id))
				{
					auto workspaceID = this->windows[window.id].GetWorkspace();
					if (workspaceID.has_value())
					{
						this->workspaces.at(workspaceID.value())
							.RemoveWindow(window.id);
					}
				}
				this->windows[window.id] = WindowInfo(
					QString::fromStdString(window.title.value_or("")),
					QString::fromStdString(window.app_id.value_or("")),
					window.workspace_id);
				if (window.is_focused && window.workspace_id.has_value())
				{
					auto& workspace
						= this->workspaces.at(window.workspace_id.value());
					workspace.SetActiveWindowId(window.id);
					workspace.AddWindow(window.id);

					this->workspaces.at(this->focusedWorkspaceID)
						.SetFocused(false);
					workspace.SetFocused(true);
					this->focusedWorkspaceID = window.workspace_id.value();
					ChangeActiveWindow(workspace, this->windows[window.id]);
				}
			},
			[this](WindowClosedEvent& event) -> void
			{
				auto workspaceID = this->windows[event.id].GetWorkspace();
				if (workspaceID.has_value())
				{
					this->workspaces.at(workspaceID.value())
						.RemoveWindow(event.id);
				}
				this->windows.erase(event.id);
			},
			[this](WindowFocusChangedEvent& event) -> void
			{
				// TODO: fix empty workspaces not being focused correctly
				auto& oldWorkspace
					= this->workspaces.at(this->focusedWorkspaceID);
				if (event.id.has_value())
				{
					oldWorkspace.SetFocused(false);
				}
				auto window = event.id.transform([this](auto& id) -> auto&
												 { return this->windows[id]; });
				if (window.has_value())
				{
					auto workspaceID = window->GetWorkspace();
					if (workspaceID.has_value())
					{
						auto& workspace
							= this->workspaces.at(workspaceID.value());
						workspace.SetActiveWindowId(event.id);
						workspace.SetFocused(true);
						ChangeActiveWindow(workspace, window);
					}
				}
			},
			[](ScreenshotCapturedEvent& /*event*/) -> void
			{
				// TODO: Send notification
			},
			[this](OverviewOpenedOrClosedEvent& event) -> void
			{ this->overviewOpen = event.is_open; },
			[](auto& /*event*/) -> void { return; }, // Default case
		};
		std::visit(eventSwitch, event.event);
	}
}
void NiriBackend::ChangeActiveWindow(const Workspace& workspace,
									 std::optional<WindowInfo&> window)
{
	if (!this->overviewOpen) [[likely]]
	{
		emit ActiveWindowChanged(workspace.GetOutput(), window);
	}
	else [[unlikely]]
	{
		auto fakeWin = WindowInfo(workspace.GetName(), "");
		emit ActiveWindowChanged(workspace.GetOutput(), fakeWin);
	}
}

} //namespace niri
