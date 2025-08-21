# Qt Ant Design Theme System

This project provides a comprehensive theme system for Qt applications based on Ant Design's color specification and dark mode guidelines.

## Features

- **Light and Dark Mode Support**: Seamlessly switch between light and dark themes
- **Ant Design Color Palette**: Full implementation of Ant Design's color system
- **Semantic Color Names**: Easy-to-use semantic color getters (primary, secondary, success, etc.)
- **Dynamic Theme Switching**: Runtime theme mode switching with signal notifications

## Theme Modes

### Light Mode
- Based on Ant Design's light theme specification
- Uses bright backgrounds with dark text
- Optimized for well-lit environments

### Dark Mode  
- Following Ant Design's dark mode guidelines from https://ant-design.antgroup.com/docs/spec/dark
- Uses dark backgrounds with light text
- Reduces eye strain in low-light conditions
- Maintains proper contrast ratios for accessibility

## Color Palette

The theme system includes the complete Ant Design color palette:

### Blue Palette (Primary)
- `Blue1` to `Blue10`: Complete blue color scale
- Automatically adapts for light/dark modes

### Gray Palette (Neutral)  
- `Gray1` to `Gray13`: Comprehensive neutral color scale
- Provides proper contrast for text and backgrounds

### Semantic Colors
- `Success`: Green color for success states
- `Warning`: Orange color for warning states  
- `Error`: Red color for error states
- `Info`: Blue color for informational states

## Usage

### Basic Usage

```cpp
#include <lib/QtAntdTheme.h>

// Create a light theme
QtAntdTheme *lightTheme = QtAntdTheme::lightTheme();

// Create a dark theme
QtAntdTheme *darkTheme = QtAntdTheme::darkTheme();

// Get colors
QColor primaryColor = lightTheme->primary();
QColor backgroundColor = lightTheme->background();
QColor textColor = lightTheme->text();
```

### Theme Switching

```cpp
QtAntdTheme *theme = new QtAntdTheme();

// Switch to dark mode
theme->setThemeMode(Antd::Dark);

// Switch to light mode
theme->setThemeMode(Antd::Light);

// Listen for theme changes
connect(theme, &QtAntdTheme::themeModeChanged, [](Antd::ThemeMode mode) {
    qDebug() << "Theme changed to:" << (mode == Antd::Light ? "Light" : "Dark");
});
```

### Color Access Methods

#### Semantic Color Getters
```cpp
QColor primary = theme->primary();        // Primary brand color
QColor secondary = theme->secondary();    // Secondary color
QColor success = theme->success();        // Success state color
QColor warning = theme->warning();        // Warning state color
QColor error = theme->error();           // Error state color
QColor info = theme->info();             // Info state color

QColor text = theme->text();             // Primary text color
QColor textSecondary = theme->textSecondary(); // Secondary text color
QColor background = theme->background();  // Background color
QColor surface = theme->surface();       // Surface color
QColor border = theme->border();         // Border color
QColor disabled = theme->disabled();     // Disabled state color
```

#### String-based Access
```cpp
QColor color = theme->getColor("primary");
QColor hover = theme->getColor("primary-hover");
QColor active = theme->getColor("primary-active");
```

#### Enum-based Access  
```cpp
QColor blue6 = theme->getColor(Antd::Blue6);   // Primary blue
QColor gray1 = theme->getColor(Antd::Gray1);   // Lightest gray
QColor success = theme->getColor(Antd::Success); // Success color
```

### Custom Colors

```cpp
// Set custom colors
theme->setColor("customPrimary", QColor("#1890ff"));
theme->setColor("customSecondary", Antd::Blue5);

// Get custom colors
QColor custom = theme->getColor("customPrimary");
```

## Available Color Keys

### Primary Colors
- `"primary"` - Main brand color
- `"primary-hover"` - Hover state
- `"primary-active"` - Active/pressed state  
- `"primary-outline"` - Outline variant

### Secondary Colors
- `"secondary"` - Secondary brand color
- `"secondary-hover"` - Secondary hover state
- `"secondary-active"` - Secondary active state

### Text Colors
- `"text"` - Primary text
- `"text-secondary"` - Secondary text
- `"text-tertiary"` - Tertiary text
- `"text-quaternary"` - Quaternary text
- `"text-disabled"` - Disabled text

### Background Colors
- `"background"` - Main background
- `"background-secondary"` - Secondary background
- `"surface"` - Surface background
- `"surface-variant"` - Surface variant

### Border Colors
- `"border"` - Primary border
- `"border-secondary"` - Secondary border  
- `"border-variant"` - Border variant

### Status Colors
- `"success"` - Success state
- `"warning"` - Warning state
- `"error"` - Error state
- `"info"` - Information state

### Disabled States
- `"disabled"` - Disabled foreground
- `"disabled-background"` - Disabled background

## Example Application

See `examples/theme_example.cpp` for a complete demonstration of the theme system, including:

- Dynamic theme switching
- Color visualization
- Styling examples
- Signal handling

## Design Principles

Following Ant Design's dark mode principles:

1. **Comfort of Content**: Avoids highly contrasting colors that cause eye strain
2. **Consistency of Information**: Information hierarchy remains consistent across themes  
3. **Accessibility**: Maintains proper contrast ratios for readability

## Building and Running

```bash
# Configure CMake
cmake -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH} -B build

# Build  
cmake --build build --config Debug

# Run example
./build/examples/Debug/examples.exe
```

## Dependencies

- Qt 5.12 or higher
- CMake 3.16 or higher
- C++11 compiler

## Contributing

When adding new colors or features:
1. Follow Ant Design specifications
2. Ensure both light and dark mode support
3. Maintain backward compatibility
4. Add appropriate documentation
5. Test with the example application

## References

- [Ant Design Dark Mode Specification](https://ant-design.antgroup.com/docs/spec/dark)
- [Ant Design Color Palette](https://ant-design.antgroup.com/docs/spec/colors)
- [Material Design Color System](https://material.io/design/color/) (for additional inspiration)
