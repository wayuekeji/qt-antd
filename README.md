# Qt AntD Widgets

Implement Ant Design style components in Qt Widgets Way, no QML as possible

## Components

Implemented / planned:

- General: Button (implemented), Icon, Typography
- Layout: Divider (implemented), Space, Grid
- Navigation: Breadcrumb, Tabs, Pagination
- Data Entry: Input (implemented), Checkbox, Radio, Switch, Select, Slider, DatePicker
- Data Display: Badge, Tag, Tooltip, Avatar, Table
- Feedback: Alert, Message, Modal, Progress, Spin
- Other: ConfigProvider, LocaleProvider, Affix

## Theme System

A theme system inspired by Ant Design provides light/dark palettes and semantic colors
(primary, error, etc.). Components can opt-in to theme-driven colors.

See [Theme System](./THEME_SYSTEM.md) for details.

## Build

Requirements:
- Qt 5.15+ or Qt 6.x
- CMake 3.22+
- C++17 compatible compiler

Build in VS Code was recommended, you can reference README in .vscode for detail.



Or you can using CMake:

```powershell
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=$env:CMAKE_PREFIX_PATH ..
cmake --build . --config Debug
```

## Button Component
![img](README.assets/img_v3_02ph_2d4632ce-cee4-439d-8b4a-37a00f6ba17g.gif)

QtAntdButton is an Ant Design styled QPushButton.

Key features:
- Types: Primary, Default, Dashed, Text, Link
- Sizes: Small, Medium, Large
- Shapes: DefaultShape (rounded rect), Round (pill), Circle
- States: block, ghost, danger, loading (with animated spinner)
- Color presets: PresetDefault, PresetPrimary, PresetDanger, PresetPink, PresetPurple, PresetCyan
- Theme-aware colors when useThemeColors() is true (default)


Basic usage:

```cpp
#include <qtantd/qtantdbutton.h>

auto *btn = new QtAntdButton("Primary");
btn->setButtonType(QtAntdButton::Primary);
btn->setButtonSize(QtAntdButton::Medium);
btn->setButtonShape(QtAntdButton::DefaultShape);
btn->setBlock(false);
btn->setGhost(false);
btn->setDanger(false);
btn->setLoading(false);
btn->setColorPreset(QtAntdButton::PresetPrimary);
```

Loading state:

```cpp
btn->setLoading(true); // shows animated spinner and hides icon
```

Circle and round shapes:

```cpp
btn->setButtonShape(QtAntdButton::Round);   // pill
btn->setButtonShape(QtAntdButton::Circle);  // circle, sized by min(width,height)
```

Link/Text style:

```cpp
btn->setButtonType(QtAntdButton::Text);
btn->setButtonType(QtAntdButton::Link);
```

## Input Component

QtAntdInput is an Ant Design styled QLineEdit.

Key features:
- Sizes: Small, Medium, Large
- Status States: Normal, Error, Warning, Success
- Clear button functionality
- Prefix/suffix text support
- Theme-aware colors when useThemeColors() is true (default)

Basic usage:

```cpp
#include <qtantd/qtantdinput.h>

auto *input = new QtAntdInput("Enter text here");
input->setInputSize(QtAntdInput::Medium);
input->setInputStatus(QtAntdInput::Normal);
input->setClearButtonEnabled(true);
```

Sizes and status:

```cpp
input->setInputSize(QtAntdInput::Large);
input->setInputStatus(QtAntdInput::Error);
```

Prefix/suffix support:

```cpp
input->setPrefixText("$");
input->setSuffixText(".com");
```

Clear button:

```cpp
input->setClearButtonEnabled(true);
connect(input, &QtAntdInput::clearButtonClicked, [](){
    qDebug() << "Clear button clicked!";
});
```

## Examples

See the examples/ folder for a demo application and settings editors.