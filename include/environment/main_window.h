#pragma once

#include <QGraphicsScene>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QWidget>
#include <QtGui>
#include <memory>
#include <mutex>
#include "cheat_db_handler2.h"
#include "cheating/cheat_window.h"
#include "gamepad.h"
#include "interfaces/keyboard_interface.h"
#include "interfaces/screen_interface.h"
#include "nes.h"

enum ActionRole : int { Host = 1 << 0, Guest = 1 << 1, None = 1 << 2 };

class MainWindow : public QMainWindow, public NES::ScreenInterface {
    Q_OBJECT

signals:
    void key_pressed(Qt::Key key);
    void key_released(Qt::Key key);
    void closed();

public:
    MainWindow();
    ~MainWindow() override = default;

    void closeEvent(QCloseEvent *event) override;

    // Keyboard methods
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    // Screen methods
    ScreenInterface *get_screen_interface();
    void set_pixel(int row, int column, NES::Color color) override;
    void refresh_screen() override;

    void clear();

    void enable_actions(ActionRole) const;

    QAction *m_load_act;
    QAction *m_reset_act;
    QAction *m_pause_act;

    QAction *m_save_to_act;
    QAction *m_load_from_act;
    QAction *m_quickload_act;
    QAction *m_quicksave_act;

    QAction *m_open_pl1_keymap_act;
    QAction *m_open_pl2_keymap_act;
    QAction *m_open_player_select_act;

    QAction *m_become_host_act;
    QAction *m_become_guest_act;
    QAction *m_run_server_act;
    QAction *m_connect_to_host_act;

    QAction *m_player1_single_act;
    QAction *m_player2_single_act;
    QAction *m_coop_player_act;
    QAction *m_mem_search_act;

private:
    void create_menus();
    void create_actions();
    QMenu *nes_menu;
    QLabel *m_image_label;
    QImage m_screen_image;

    QMenu *m_nes_menu;
    QMenu *m_saves_menu;
    QMenu *m_settings_menu;
    QMenu *m_profile_menu;
    QMenu *m_network_menu;
    QActionGroup *m_profile_group;
    std::vector<std::pair<QAction *, int>> m_all_actions;

    QTimer m_clock;
    bool pause = false;

    static QMap<Qt::Key, int> m_index_by_key;
    std::atomic<uint8_t> m_pressed_keys_bitset{};
};
