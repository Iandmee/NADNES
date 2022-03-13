#include "Controller.h"
#include "ControllerExceptions.h"

namespace NES {

uint8_t Controller::CpuRead(uint16_t addr) {
    uint8_t index = addr - 0x4016;
    uint8_t ans = m_snapshot[index] & 1;
    m_snapshot[index] >>= 1;
    return ans;
}

void Controller::CpuWrite(uint16_t addr, uint8_t value) {
    if (addr != 0x4016 || (value & 1) != 1) {
        throw InvalidControllerWrite(addr, value);
    }
    m_snapshot[0] = m_keyboardInterface->getPressedKeys();
    m_snapshot[1] = 0;  // Something
}

void Controller::connect(KeyboardInterface *keyboardInterface, ConnectToken) {
    m_keyboardInterface = keyboardInterface;
}

}  // namespace NES