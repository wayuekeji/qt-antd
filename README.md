# Qt AntD Widgets

Implement Ant Design style components in Qt Widgets Way, no QML as possible

## Components

TODO:

- General：Button、Icon、Typography
- Layout：Divider、Space、Grid
- Navigation：Breadcrumb、Tabs、Pagination
- Data Entry：Input、Checkbox、Radio、Switch、Select、Slider、DatePicker
- Data Display：Badge、Tag、Tooltip、Avatar、Table
- Feedback：Alert、Message、Modal、Progress、Spin
- Other：ConfigProvider、LocaleProvider、Affix

## Theme System

Qt AntD Widgets provides a comprehensive theme system that allows you to customize the appearance of all components consistently across your application.

### Architecture

The theme system consists of three main classes:

- **`QtAntdTheme`** - Manages theme colors and provides color palette functionality
- **`QtAntdStyle`** - Singleton style manager that integrates with Qt's style system
- **`QtAntdThemePrivate`** - Internal implementation for theme management

### Color Management

#### Predefined Color Palette

```cpp
namespace Antd {
enum Color {
    lightWhite
};
}
```

#### Default Theme Colors

The default theme defines semantic color keys that components use:

```cpp
QtAntdTheme::QtAntdTheme() {
    setColor("primary1", Antd::lightWhite);      // Primary color
}
```

### Usage

#### Accessing the Theme System

```cpp
#include <QtAntdStyle.h>
#include <QtAntdTheme.h>

// Get the global style instance
QtAntdStyle &style = QtAntdStyle::instance();

// Get theme colors
QColor primaryColor = style.themeColor("primary1");
```

#### Customizing Theme Colors

```cpp
// Get the current theme
QtAntdTheme *theme = new QtAntdTheme();

// Set custom colors using predefined palette
theme->setColor("primary1", Antd::blue500);
theme->setColor("text", Antd::darkBlack);

// Set custom colors using QColor
theme->setColor("border", QColor(0, 0, 0, 50));

// Apply the theme to the style system
QtAntdStyle::instance().setTheme(theme);
```

#### Component Theme Integration

Components automatically use theme colors when `useThemeColors` is enabled (default):

```cpp
QtAntdDivider *divider = new QtAntdDivider("Example");

// Enable/disable theme colors
divider->setUseThemeColors(true);  // Use theme colors (default)
divider->setUseThemeColors(false); // Use custom colors

// When theme colors are enabled, these colors come from the theme:
QColor lineColor = divider->lineColor();  // Returns theme's "border" color
QColor textColor = divider->textColor();  // Returns theme's "text" color

// When theme colors are disabled, you can set custom colors:
divider->setLineColor(QColor("#ff0000"));
divider->setTextColor(QColor("#0000ff"));
```