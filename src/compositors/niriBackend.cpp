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

	connect(&cmdSock, &QLocalSocket::connected,
			[]() -> void
			{
				// cmdSock.write("{\"Action\":{\"FocusWorkspace\":{\"reference\":{"
				// 			  "\"Index\":1}}}}\n");
			});
	cmdSock.connectToServer(sockAddr);
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
void NiriBackend::ProcessMessage(const std::string_view message)
{
	niri::Response response;
	auto err = glz::read<glzOpts{}>(response, message);
	if (err.ec != glz::error_code::none)
	{
#ifndef __clangd__
		// qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
	}
	else
	{
		auto out = glz::write<glzOpts{}>(response).value_or("ERROR");
		qDebug().noquote() << out;
	}
	niri::Event event;
	err = glz::read<glzOpts{}>(event, message);
	if (err.ec != glz::error_code::none)
	{
#ifndef __clangd__
		// qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
	}
	else
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
						workspace.SetFocused(data.is_focused);
						workspace.SetActive(data.is_active);
						workspace.SetFocused(data.is_focused);
						workspace.SetActiveWindowId(data.active_window_id);
					}
					if (data.is_active || data.is_focused)
					{
						this->activeWorkspaces[workspace.GetOutput()] = data.id;
						auto window = workspace.GetActiveWindowID().transform(
							[this](auto id) -> auto&
							{ return this->windows[id]; });
						emit ActiveWindowChanged(workspace.GetOutput(), window);
					}
					workspaceGroups[output].push_back(&workspace);
				}
				namespace rv = std::views;
				auto openWorkspaces
					= event.workspaces
					  | rv::transform([](auto& value) -> uint64_t
									  { return value.id; })
					  | std::ranges::to<std::unordered_set<uint64_t>>();
				std::erase_if(this->workspaces,
							  [&openWorkspaces](auto& pair) -> bool
							  { return !openWorkspaces.contains(pair.first); });
				for (const auto& [key, val] : workspaceGroups.asKeyValueRange())
				{
					std::ranges::sort(val, {}, &Workspace::GetIndex);
					emit this->WorkspacesChanged(key, val);
				}
			},
			[this](WorkspaceUrgencyChangedEvent& event) -> void
			{ this->workspaces[event.id].SetUrgent(event.urgent); },
			[this](WorkspaceActivatedEvent& event) -> void
			{
				auto& workspace = this->workspaces[event.id];
				this->workspaces[activeWorkspaces[workspace.GetOutput()]]
					.SetActive(false);
				this->activeWorkspaces[workspace.GetOutput()] = event.id;
				workspace.SetActive(true);
				auto window = workspace.GetActiveWindowID().transform(
					[this](auto id) -> auto& { return this->windows[id]; });
				emit ActiveWindowChanged(workspace.GetOutput(), window);
				if (event.focused)
				{
					workspace.SetFocused(true);
					this->focusedWorkspaceID = event.id;
				}
			},
			[this](WorkspaceActiveWindowChangedEvent& event) -> void
			{
				auto& key = this->workspaces[event.workspace_id].GetOutput();
				auto window = event.active_window_id.transform(
					[this](auto& id) -> auto& { return this->windows[id]; });
				emit ActiveWindowChanged(key, window);
			},
			[this](WindowsChangedEvent& event) -> void
			{
				for (auto& window : event.windows)
				{
					this->windows[window.id] = WindowInfo(
						QString::fromStdString(window.title.value_or("")),
						QString::fromStdString(window.app_id.value_or("")));
					if (window.is_focused && window.workspace_id.has_value())
					{
						auto& workspace
							= this->workspaces[window.workspace_id.value()];
						workspace.SetActiveWindowId(window.id);
						workspace.SetFocused(true);
						this->workspaces[this->focusedWorkspaceID].SetFocused(
							false);
						this->focusedWorkspaceID = window.workspace_id.value();
						emit ActiveWindowChanged(workspace.GetOutput(),
												 this->windows[window.id]);
					}
				}
			},
			[this](WindowOpenedOrChangedEvent& event) -> void
			{
				auto& window{event.window};
				this->windows[window.id] = WindowInfo(
					QString::fromStdString(window.title.value_or("")),
					QString::fromStdString(window.app_id.value_or("")));
				if (window.is_focused && window.workspace_id.has_value())
				{
					auto& workspace
						= this->workspaces[window.workspace_id.value()];
					workspace.SetActiveWindowId(window.id);
					workspace.SetFocused(true);
					this->workspaces[this->focusedWorkspaceID].SetFocused(
						false);
					emit ActiveWindowChanged(workspace.GetOutput(),
											 this->windows[window.id]);
					this->focusedWorkspaceID = window.workspace_id.value();
				}
			},
			[this](WindowClosedEvent& event) -> void
			{ this->windows.erase(event.id); },
			[this](WindowFocusChangedEvent& event) -> void
			{
				auto& workspace = this->workspaces[this->focusedWorkspaceID];
				if (event.id.has_value())
				{
					workspace.SetActiveWindowId(event.id);
				}
				auto window = event.id.transform([this](auto& id) -> auto&
												 { return this->windows[id]; });
				emit ActiveWindowChanged(workspace.GetOutput(), window);
			},
			[](ScreenshotCapturedEvent& event) -> void
			{
				// TODO: Send notification
			},
			[](auto& /*event*/) -> void { return; }, // Default case
		};
		std::visit(eventSwitch, event.event);
	} //namespace niri
}

} //namespace niri
