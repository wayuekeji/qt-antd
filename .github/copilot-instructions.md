# Antd Style Components

Antd Style Components is a C++ library providing Ant Design components for Qt6/Qt5 applications. 
The project implements Ant Design style widgets in native Qt C++ without using QML and Minimize the use of QSS.
It support CMake build systems only.

## Project Structure

```
.
├── CMakeLists.txt                # Main CMake configuration
├── README.md                     # Project overview
├── THEME_SYSTEM.md               # Theme system documentation
├── components/                   # Library source code
│   ├── CMakeLists.txt            # Component library build configuration
│   ├── qtantddivider_p.h         # Private header for divider component
│   ├── qtantddivider.cpp         # Implementation of divider component
│   └── antdlib/                  # Core styling and theme system
│       ├── qtantdstyle_p.h       # Private style header
│       ├── qtantdstyle.cpp       # Style implementation
│       ├── qtantdtheme_p.h       # Private theme header
│       └── qtantdtheme.cpp       # Theme implementation
├── examples/                     # Example applications
│   ├── CMakeLists.txt            # Example application build config
│   ├── main.cpp                  # Example application entry point
│   ├── mainwindow.cpp            # Example main window implementation
│   └── mainwindow.h              # Example main window header
└── include/                      # Public headers
    └── qtantd/
        ├── components_global.h   # Export macros
        ├── qtantddivider.h       # Public divider component API
        └── lib/                  # Public API for style/theme
```

## Component Development Guidelines

### Component Structure
Each component typically consists of three files:
1. Public header in `include/qtantd/` (e.g., `qtantddivider.h`)
2. Private header in `components/` (e.g., `qtantddivider_p.h`)
3. Implementation in `components/` (e.g., `qtantddivider.cpp`)

### Naming Convention
- Class names should be prefixed with `QtAntd` (e.g., `QtAntdDivider`)
- Private classes should be suffixed with `Private` (e.g., `QtAntdDividerPrivate`)
- Files should use lowercase names (e.g., `qtantddivider.h`)

### Adding a New Component
1. Create the three required files following the naming convention
2. Update the `components/CMakeLists.txt` to include your new files in the appropriate variables:
   - Add public headers to the `HEADER` variable
   - Add private headers to the `COMPONENTSHEADER` variable
   - Add implementation files to the `COMPONENTSSOURCE` variable
3. Implement the component following the existing pattern
4. Add an example usage in the `examples/` directory

## Theme System

The project implements a comprehensive theme system based on Ant Design's color specification:

- Light and dark mode support
- Complete Ant Design color palette
- Semantic color naming (primary, secondary, etc.)
- Dynamic theme switching

See `THEME_SYSTEM.md` for detailed documentation.

## Building the Project

### Requirements
- Qt 5.15+ or Qt 6.x
- CMake 3.22+
- C++17 compatible compiler

### Using CMake (Recommended)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Using qmake
```bash
qmake
make # or nmake/mingw32-make on Windows
```

## Development Workflow

1. Implement or modify components following the component structure guidelines
2. Update the examples to showcase your changes
3. Build and test the project
4. Submit your changes

## Testing

Currently, the project relies on manual testing through the examples. When adding new functionality, please:
1. Add an appropriate demonstration in the examples
2. Test on both light and dark themes
3. Ensure compatibility with both Qt5 and Qt6 when possible

## Roadmap

The project aims to implement the following components as documented in the README:
- General: Button, Icon, Typography
- Layout: Divider (implemented), Space, Grid
- Navigation: Breadcrumb, Tabs, Pagination
- Data Entry: Input, Checkbox, Radio, Switch, Select, Slider, DatePicker
- Data Display: Badge, Tag, Tooltip, Avatar, Table
- Feedback: Alert, Message, Modal, Progress, Spin
- Other: ConfigProvider, LocaleProvider, Affix

## Common Issues and Solutions

### Build Issues
- If CMake cannot find Qt, set CMAKE_PREFIX_PATH to your Qt installation
- On Windows, ensure you are using the correct generator for your Visual Studio version

### Component Development
- When creating custom widgets, ensure to use `setBackgroundRole` and `setAutoFillBackground` appropriately for proper theme support
- Use the theme system for colors instead of hardcoded values to ensure proper light/dark mode support