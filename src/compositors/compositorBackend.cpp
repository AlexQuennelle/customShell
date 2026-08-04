#include "compositors/compositorBackend.h"

WindowInfo::WindowInfo(QString title, QString appID) :
	title(std::move(title)), appID(std::move(appID))
{ }
auto WindowInfo::GetTitle() const -> const QString& { return this->title; }
auto WindowInfo::GetAppID() const -> const QString& { return this->appID; }
void WindowInfo::SetTitle(const QString& newTitle)
{
	this->title = newTitle;
	emit TitleChanged(this->title);
}
void WindowInfo::SetAppID(const QString& newAppID)
{
	this->appID = newAppID;
	emit AppIDChanged(this->appID);
}
auto WindowInfo::operator=(WindowInfo&& other) noexcept -> WindowInfo&
{
	this->title = other.title;
	this->appID = other.appID;

	emit TitleChanged(this->title);
	emit AppIDChanged(this->appID);

	return *this;
};
