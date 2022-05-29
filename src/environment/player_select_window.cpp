#include "environment/player_select_window.h"
#include <QMessageBox>
#include <QtUiTools>

PlayerManager::PlayerManager() {
    QFile file("../UI/player_select.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    loader.load(&file, this);
    QMetaObject::connectSlotsByName(this);

    m_player1_select = findChild<QComboBox *>("player1_select");
    m_player2_select = findChild<QComboBox *>("player2_select");
    m_ok_btn = findChild<QPushButton *>("ok_btn");
    m_cancel_btn = findChild<QPushButton *>("cancel_btn");
    make_connections();
}

void PlayerManager::on_ok_btn_clicked() {
    int player1_cur = m_view_ids[m_player1_select->currentIndex()];
    int player2_cur = m_view_ids[m_player2_select->currentIndex()];
    if (player1_cur == player2_cur && m_player2_select->isEnabled()) {
        QMessageBox::warning(this, "Players matched", "Players must differ!", (QMessageBox::Ok));
        return;
    }
    m_player1_id = player1_cur;
    m_player2_id = player2_cur;
    update_gamepads();
    close();
}

void PlayerManager::on_cancel_btn_clicked() {
//    m_mutex
    size_t m_player1_index =
        std::find(m_view_ids.begin(), m_view_ids.end(), m_player1_id) - m_view_ids.begin();
    size_t m_player2_index =
        std::find(m_view_ids.begin(), m_view_ids.end(), m_player2_id) - m_view_ids.begin();

    m_player2_select->setCurrentIndex(int(m_player2_index));
    m_player1_select->setCurrentIndex(int(m_player1_index));
    close();
}

void PlayerManager::add_player(int index, const QString &name) {
//    std::unique_lock lock(m_mutex);
    m_name_by_id[index] = name;
    update_view_and_indexes();
}

void PlayerManager::remove_player(int index) {
//    std::unique_lock lock(m_mutex);
    if (index <= 1) {
        qDebug() << QString("Removed index %1").arg(index);
        return;
    }
    if (m_player1_id == index) {
        m_player1_id = (0 == m_player2_id ? 1 : 0);
    } else if (m_player2_id == index) {
        m_player2_id = (0 == m_player1_id ? 1 : 0);
    }
    update_gamepads();
    m_screens_map.erase(index);
    m_gamepads_map.erase(index);
    m_screens_map.erase(index);
    update_view_and_indexes();
}

void PlayerManager::show_settings() {
    show();
}

void PlayerManager::add_screen(int id, NES::ScreenInterface *screen) {
    assert(m_screens_map.count(id) == 0);
    m_screens_map[id] = screen;
}

void PlayerManager::add_keyboard(int id, NES::KeyboardInterface *gamepad) {
    assert(m_gamepads_map.count(id) == 0);
    m_gamepads_map[id] = gamepad;
}

void PlayerManager::update_view_and_indexes() {
//    std::unique_lock lock(m_mutex);
    m_view_ids.clear();
    for (const auto &id_ptr : m_gamepads_map) {
        m_view_ids.push_back(id_ptr.first);
    }

    for (int player = 1; player <= 2; ++player) {
        QComboBox *comboBox = (player == 1 ? m_player1_select : m_player2_select);
        comboBox->clear();
        for (int id : m_view_ids) {
            QString name = (m_name_by_id.count(id) ? QString(tr("name: %1")).arg(m_name_by_id[id]) :
                            QString(tr("id: %1")).arg(id));
            comboBox->addItem(name);
        }
    }
}

void PlayerManager::set_pixel(int row, int column, NES::Color color) {
//    std::unique_lock lock(m_mutex);
    for (const auto &pr : m_screens_map) {
        pr.second->set_pixel(row, column, color);
    }
}

void PlayerManager::refresh_screen() {
//    std::unique_lock lock(m_mutex);
    for (const auto &pr : m_screens_map) {
        pr.second->refresh_screen();
    }
}

void PlayerManager::make_connections() {
    connect(m_ok_btn, SIGNAL(clicked()), this, SLOT(on_ok_btn_clicked()));
    connect(m_cancel_btn, SIGNAL(clicked()), this, SLOT(on_cancel_btn_clicked()));
}

NES::KeyboardInterface *PlayerManager::get_gamepad_wrapper1() {
    return dynamic_cast<NES::KeyboardInterface *>(&m_gamepad_wrapper1);
}

NES::KeyboardInterface *PlayerManager::get_gamepad_wrapper2() {
    return dynamic_cast<NES::KeyboardInterface *>(&m_gamepad_wrapper2);
}

NES::ScreenInterface *PlayerManager::get_screen() {
    return dynamic_cast<NES::ScreenInterface *>(this);
}

void PlayerManager::update_gamepads() {
//    std::unique_lock lock(m_mutex);
    m_gamepad_wrapper1.set_gamepad(m_gamepads_map[m_player1_id]);
    m_gamepad_wrapper2.set_gamepad(m_gamepads_map[m_player2_id]);
}

uint8_t PlayerManager::GamepadWrapper::get_pressed_keys() const {
    return m_gamepad->get_pressed_keys();
}
void PlayerManager::GamepadWrapper::set_gamepad(NES::KeyboardInterface *new_gamepad) {
    m_gamepad = new_gamepad;
}
