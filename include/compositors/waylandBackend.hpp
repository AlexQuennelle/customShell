#pragma once

#include "compositors/compositorBackend.h"

#include <qguiapplication_platform.h>
#include <qnativeinterface.h>

class WaylandBackend : public ICompositorBackend
{
	Q_OBJECT; // NOLINT

	public:
	WaylandBackend();
	~WaylandBackend() override = default;

	auto GetActiveWindow(QStringView outputName) -> WindowInfo& override
	{
		return this->activeWindow;
	}

	private:
	wl_display* display{nullptr};
	wl_compositor* compositor{nullptr};
	WindowInfo activeWindow;
};
