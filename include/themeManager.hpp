#pragma once

#include <QColor>
#include <QObject>
#include <QtQmlIntegration>

class ThemeManager : public QObject
{
	Q_OBJECT; // NOLINT
	QML_ELEMENT
	QML_SINGLETON;

	Q_PROPERTY(QColor background READ GetBackground NOTIFY themeChanged)
	Q_PROPERTY(QColor textCol READ GetTextCol NOTIFY themeChanged)
	Q_PROPERTY(QColor selected READ GetSelected NOTIFY themeChanged)
	Q_PROPERTY(QColor fill READ GetFill NOTIFY themeChanged)
	Q_PROPERTY(QColor empty READ GetEmpty NOTIFY themeChanged)
	Q_PROPERTY(QColor hover READ GetHover NOTIFY themeChanged)

	public:
	ThemeManager() = default;
	ThemeManager(const ThemeManager&) = delete;
	ThemeManager(ThemeManager&&) = delete;
	~ThemeManager() override = default;

	auto GetBackground() const -> QColor { return this->background; }
	auto GetTextCol() const -> QColor { return this->textCol; }
	auto GetSelected() const -> QColor { return this->selected; }
	auto GetFill() const -> QColor { return this->fill; }
	auto GetEmpty() const -> QColor { return this->empty; }
	auto GetHover() const -> QColor { return this->hover; }

	auto operator=(const ThemeManager&) -> ThemeManager& = delete;
	auto operator=(ThemeManager&&) -> ThemeManager& = delete;

	signals:
	void themeChanged();

	private:
	QColor background{0x26233a};
	QColor textCol{0xe0def4};
	QColor selected{0xc4a7e7};
	QColor fill{0x6e6a86};
	QColor empty{0x44415a};
	QColor hover{0x908caa};
};
