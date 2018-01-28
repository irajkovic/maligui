#include <QApplication>

#include <functional>

#include "mali-gui/Stacker.hpp"
#include "mali-gui/Button.hpp"
#include "mali-gui/Layout.hpp"
#include "mali-gui/FontDatabase.hpp"
#include "qt-wrapper/device_emulator.hpp"

int applyOperation(int arg1, int arg2, char operation)
{
    switch (operation) {
    case '+':
        return arg1 + arg2;
    case '-':
        return arg1 - arg2;
    case '*':
        return arg1 * arg2;
    case '/':
        return arg1 / arg2;
    default:
        return arg2;
    }
}

bool handleNumpadBtnClick(maligui::Button<QColor> *displayPtr,
                          maligui::Widget<QColor, int>*w,
                          maligui::Point<int> p)
{
    (void)p;
    static int result = 0;
    static char operation = '\0';
    static bool clearScreen = true;

    if (displayPtr == nullptr) {
        return false;
    }

    auto button = dynamic_cast<maligui::Button<QColor, int>*>(w);
    if (button == nullptr) {
        return false;
    }

    if (clearScreen) {
        displayPtr->text("");
        clearScreen = false;
    }

    // Use button label to identify which button is clicked
    char c = button->text().at(0);
    if (c >= '0' && c <= '9') {
        displayPtr->text( displayPtr->text() + c );
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') {
        clearScreen = true;

        try {
            int tmp = std::stoi(displayPtr->text());
            result = applyOperation(result, tmp, operation);
        }
        catch (std::invalid_argument e) {
            ; // Do nothing. The display was just cleared.
        }

        displayPtr->text(std::to_string(result));

        operation = c;
    }
    else {  // DEL pressed
        displayPtr->text("0");
        clearScreen = true;
        result = 0;
    }

    return false;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Emulate 240x320 device. Stacker is our interface to the Gui system.
    maligui::Stacker<QColor> stacker(std::make_shared<DeviceEmulator>(240, 320));

    // Main window Widget, use all available screen space
    auto window = std::make_unique<maligui::Widget<QColor>>();
    window->geometry({0, 0, stacker.width(), stacker.height()});

    // Create Calculator display. Make it a button, so we can make it to reset
    // the result to zero when clicked.
    auto display = std::make_unique<maligui::Button<QColor>>();
    display->geometry(maligui::Rectangle<int>{10, 20, stacker.width() - 20, 40});
    display->backgroundColor(QColor(Qt::yellow));
    display->foregroundColor(Qt::black);
    display->horizontalAlign(maligui::align::Horizontal::RIGHT);
    display->setFont("Century Schoolbook L", 24);
    display->text("0");

    // On click handler for numpad buttons. Make sure it is defined *before*
    // the display is added to the main window, as otherwise the display smart
    // pointer would be invalid. We know our display object will live throughout
    // the application, so it's safe to do so.
    auto numpadBtnClickHandler = std::bind(handleNumpadBtnClick,
                                           display.get(),
                                           std::placeholders::_1,
                                           std::placeholders::_2);

    // Add the display to the main winow and let it take the ownership.
    window->addChild(std::move(display));

    // Create the numpad area.
    auto numpad = std::make_unique<maligui::Widget<QColor>>();
    numpad->geometry(maligui::Rectangle<int>{10, 80, stacker.width() - 20, 230});

    // Create 12 labels for our numpad buttons.
    std::vector<std::string> labels = {
        "1", "2", "3", "+",
        "4", "5", "6", "-",
        "7", "8", "9", "*",
        "0", "C", "=", "/"
    };

    // Now create a numpad button maligui::Widget for each label
    for (auto& label : labels) {
        auto button = std::make_unique<maligui::Button<QColor>>();
        button->text(label);
        button->backgroundColor(QColor("#555555"));
        button->foregroundColor(QColor("#CCCCCC"));
        button->onClickHandler(numpadBtnClickHandler);
        button->setFont("Consolas", 18);
        numpad->addChild(std::move(button));
    }

    // Align numpad buttons into the 4x4 grid, with 5px padding.
    maligui::makeGrid<QColor>(numpad.get(),  /* Parent maligui::Widget */
                              4,             /* Num horizontal */
                              4,             /* Num vertical */
                              5,             /* Horizontal pading */
                              5);            /* Vertical padding */

    // We don't need numpad anymore, we can add it to the main window.
    window->addChild(std::move(numpad));

    // Finaly, put the created mainwindow widget at the top of our display. It
    // will now be the active widget, receiving click events.
    stacker.push(window.get());

    return a.exec();
}


