#pragma once

#include <QObject>
#include <string>
#include "Controller.h"
#include "KeyboardInterface.h"
#include "PPU.h"
#include "R6502.h"
#include "ScreenInterface.h"
#include "bus.h"
#include "cartridge.h"

namespace NES {
struct Nes : public QObject {
    Q_OBJECT
public:
    Nes(const std::string &filename, ScreenInterface *screen_, KeyboardInterface *keyboard_);
    ~Nes() override = default;

signals:

public slots:
    void tick();

private:
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
    Bus bus;
    Controller controller;
};
}  // namespace NES
