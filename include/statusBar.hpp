#pragma once

#include "compositorBackend.h"

#include <LayerShellQt/Window>
#include <QQuickView>

class StatusBar : public QQuickView
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(WindowInfo* activeWindow READ GetActiveWindow NOTIFY
				   ActiveWindowChanged)

	public:
	StatusBar(QWindow* parent = nullptr);

	void Init();

	auto GetActiveWindow() -> WindowInfo*;
	void SetActiveWindow(std::optional<WindowInfo&> win)
	{
		this->activeWindow = win;
		emit ActiveWindowChanged(
			this->activeWindow
				.transform([](auto& val) -> auto* { return &val; })
				.value_or(nullptr));
	}

	signals:
	void ActiveWindowChanged(WindowInfo* newWin);

	private:
	std::optional<WindowInfo&> activeWindow;
};
