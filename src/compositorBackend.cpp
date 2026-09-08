#include "compositorBackend.h"

WindowInfo::WindowInfo(QString title, QString appID) :
	title(std::move(title)), appID(std::move(appID))
{ }
auto WindowInfo::GetTitle() const -> const QString& { return this->title; }
auto WindowInfo::GetAppID() const -> const QString& { return this->appID; }
void WindowInfo::SetTitle(const QString& newTitle)
{
	this->title = newTitle;
	emit this->TitleChanged(this->title);
}
void WindowInfo::SetAppID(const QString& newAppID)
{
	this->appID = newAppID;
	emit this->AppIDChanged(this->appID);
}
auto WindowInfo::operator=(const WindowInfo& other) -> WindowInfo&
{
	if (&other == this)
		return *this;

	this->title = other.title;
	this->appID = other.appID;

	emit this->TitleChanged(this->title);
	emit this->AppIDChanged(this->appID);

	return *this;
};
auto WindowInfo::operator=(WindowInfo&& other) noexcept -> WindowInfo&
{
	this->title = other.title;
	this->appID = other.appID;

	emit this->TitleChanged(this->title);
	emit this->AppIDChanged(this->appID);

	return *this;
};
Workspace::Workspace(QString id, QString name, QString output,
					 std::optional<uint64_t> windowID, uint8_t index,
					 bool urgent, bool active, bool focused) :
	id(std::move(id)),
	name(std::move(name)),
	output(std::move(output)),
	activeWindowID(windowID),
	index(index),
	urgent(urgent),
	active(active),
	focused(focused)
{ }

auto Workspace::GetID() const -> const QString& { return this->id; }
auto Workspace::GetName() const -> const QString& { return this->name; }
auto Workspace::GetOutput() const -> const QString& { return this->output; }
auto Workspace::GetActiveWindowID() const -> std::optional<uint64_t>
{
	return this->activeWindowID;
}
auto Workspace::GetIndex() const -> uint8_t { return this->index; }
auto Workspace::GetUrgent() const -> bool { return this->urgent; }
auto Workspace::GetActive() const -> bool { return this->active; }
auto Workspace::GetFocused() const -> bool { return this->focused; }
auto Workspace::GetEmpty() const -> bool { return this->empty; }
auto Workspace::GetDead() const -> bool { return this->dead; }

void Workspace::SetName(const QString& name)
{
	this->name = name;
	emit this->nameChanged(this->name);
}
void Workspace::SetOutput(const QString& output) { this->output = output; }
void Workspace::SetActiveWindowId(const std::optional<uint64_t> windowID)
{
	this->activeWindowID = windowID;
}
void Workspace::SetIndex(uint8_t index)
{
	this->index = index;
	emit this->indexChanged(this->index);
}
void Workspace::SetUrgent(bool urgent)
{
	this->urgent = urgent;
	emit this->urgentChanged(this->urgent);
}
void Workspace::SetActive(bool active)
{
	this->active = active;
	emit this->activeChanged(this->active);
}
void Workspace::SetFocused(bool focused)
{
	this->focused = focused;
	emit this->focusedChanged(this->focused);
}
void Workspace::SetEmpty(bool empty)
{
	this->empty = empty;
	emit this->emptyChanged(this->empty);
}

void Workspace::SetDead() { this->dead = true; };
