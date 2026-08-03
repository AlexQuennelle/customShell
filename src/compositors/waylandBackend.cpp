#include "compositors/waylandBackend.hpp"

#include <qscreen.h>

WaylandBackend::WaylandBackend()
{
	namespace qni = QNativeInterface;
	auto* app = qGuiApp;
	if (auto* wayland = app->nativeInterface<qni::QWaylandApplication>())
	{
		this->display = wayland->display();
		this->compositor = wayland->compositor();
	}
	else
	{
		qFatal() << "ERROR: Failed to get wayland interface";
	}
	for (auto* screen : QGuiApplication::screens())
	{
		qDebug() << screen->name();
	}
};
