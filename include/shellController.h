#pragma once

#include "shellBackend.h"
#include "statusBar.hpp"

#include <qobject.h>
#include <qquickview.h>

class ShellController : public QObject
{
	Q_OBJECT; // NOLINT

	public:
	explicit ShellController(ShellBackend& backend, QObject* parent = nullptr);

	private:
	void InitStatusBars();

	void OnActiveWindowChanged(const QString& output,
							   std::optional<WindowInfo&> window);

	std::optional<ShellBackend&> backend;
	std::map<QString, std::unique_ptr<StatusBar>> statusBars;
};
